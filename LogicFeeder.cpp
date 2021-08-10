#include "LogicFeeder.h"
#include "SessionEvent.h"

using namespace std;

LogicFeeder::LogicFeeder(wxEvtHandler *handler, int id, const LogicFeederConfig &config):
    evtHandler_(handler),
    id_(id),
    sweepPoints_(nullptr),
    vals_(nullptr),
    parallelChannelsPerSample_(config.parallelChannelsPerSample),
    numTotalChannels_(config.numTotalChannels),
    channelIdx_(0),
    blocksize_(config.blocksize)
{
    bitsPerSample_ = config.bitsPerSample / config.parallelChannelsPerSample;

    divider_ = pow(2.0, ((double)bitsPerSample_-1.0));
    signBitPosition_= bitsPerSample_;
}

LogicFeeder::~LogicFeeder()
{
    freeBuffer();
}

void LogicFeeder::data_feed_callback(std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet)
{
    if(packet->type()->id() == SR_DF_LOGIC)
    {
        std::shared_ptr<sigrok::Logic> logicPayload = std::dynamic_pointer_cast<sigrok::Logic>(packet->payload());
        uint8_t *pBuffer = reinterpret_cast<uint8_t*>(logicPayload->data_pointer());

        for(size_t i = 0; i < logicPayload->data_length() ; i += logicPayload->unit_size())
        {
            uint64_t raw_sample = 0;
            for(size_t k = 0 ; k < logicPayload->unit_size() ; ++k)
                raw_sample |= static_cast<uint64_t>(*pBuffer++) << 8*k;

            for (size_t j = 0 ; j < parallelChannelsPerSample_ ; ++j)
            {
                int64_t new_sample = convertToSigned(raw_sample);
                raw_sample >>= bitsPerSample_;

                appendToBuffer(static_cast<double>(new_sample) / divider_);
            }
        }
    }
    else if(packet->type()->id() == SR_DF_END)
    {
        freeBuffer();
        wxQueueEvent(evtHandler_, new SessionEvent(wxEVT_SESSION_ENDED, id_));
    }
}

int64_t LogicFeeder::convertToSigned(uint64_t raw_sample)
{
    const int64_t signBitMask       = 1 << (signBitPosition_-1);   // 0b00001000
    const int64_t validBits         = (1 << (signBitPosition_))-1; // 0b00001111
    const int64_t signExtensionBits = ~validBits;                  // 0b11110000
    int64_t data = raw_sample;

    if (data & signBitMask)
        data |= signExtensionBits;
    else
        data &= validBits;

    return data;
}

void LogicFeeder::allocateNeededBuffers()
{
    if(!sweepPoints_)
        sweepPoints_ = new size_t[numTotalChannels_];
    if(!vals_)
    {
        vals_ = new double*[numTotalChannels_];

        for (size_t i = 0 ; i < numTotalChannels_ ; ++i)
        {
            double *pdata = new double[blocksize_];
            vals_[i] = pdata;
            sweepPoints_[i] = 0;
        }
    }
}

void LogicFeeder::freeBuffer()
{
    if(sweepPoints_)
        delete [] sweepPoints_;
    sweepPoints_ = nullptr;

    if (vals_)
    {
        for (size_t i = 0 ; i < numTotalChannels_ ; ++i)
            delete [] (vals_[i]);
        delete [] vals_;
    }
    vals_ = nullptr;
}

void LogicFeeder::appendToBuffer(double data)
{
    allocateNeededBuffers();

    if(!vals_ || !sweepPoints_)
        return;

    assert(sweepPoints_[channelIdx_] < blocksize_);

    double *pData = vals_[channelIdx_];

    pData[sweepPoints_[channelIdx_]] = data;
    sweepPoints_[channelIdx_] += 1;
    ++channelIdx_;
    channelIdx_ %= numTotalChannels_;

    if (channelIdx_ == 0 && sweepPoints_[0] == blocksize_)
    {
        SessionEvent *evt = new SessionEvent(wxEVT_SESSION_UPDATE, id_);
        evt->SetData(vals_, sweepPoints_, numTotalChannels_);
        wxQueueEvent(evtHandler_, evt);
        sweepPoints_ = nullptr;
        vals_ = nullptr;
    }
}

