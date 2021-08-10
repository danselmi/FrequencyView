#ifndef LADATA_FEEDER
#define LADATA_FEEDER

#include <libsigrokcxx/libsigrokcxx.hpp>

class wxEvtHandler;

struct LogicFeederConfig
{
    size_t blocksize;
    size_t bitsPerSample;
    size_t numTotalChannels;
    size_t parallelChannelsPerSample = 1;
};

class LogicFeeder
{
public:
    LogicFeeder(wxEvtHandler *handler, int id, const LogicFeederConfig &config);
    ~LogicFeeder();
    void data_feed_callback(std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet);
protected:
    void clearTraces();
    void allocateNeededBuffers();
    void freeBuffer();
    void appendToBuffer(double data);
    int64_t convertToSigned(uint64_t raw_sample);

    wxEvtHandler *evtHandler_;
    int id_;
    size_t *sweepPoints_;
    double **vals_;

    double divider_;
    size_t signBitPosition_;
    size_t bitsPerSample_;
    size_t parallelChannelsPerSample_;
    size_t numTotalChannels_;
    size_t channelIdx_;
    size_t blocksize_;
};

#endif

