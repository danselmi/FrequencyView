#include "SpectrumFeeder.h"
#include "SessionEvent.h"

using namespace std;

SpectrumFeeder::SpectrumFeeder(wxEvtHandler *handler, int id):
    sweep_points(0),
    values(nullptr),
    evtHandler_(handler),
    id_(id)
{}

void SpectrumFeeder::data_feed_callback(shared_ptr<sigrok::Device> device, shared_ptr<sigrok::Packet> packet)
{
    if(packet->type()->id() == SR_DF_FRAME_BEGIN)
    {
        sweep_points = 0;
    }
    else if(packet->type()->id() == SR_DF_ANALOG)
    {
        shared_ptr<sigrok::Analog> analogPayload = std::dynamic_pointer_cast<sigrok::Analog>(packet->payload());

        const sigrok::Quantity *mq;
        try {mq = analogPayload->mq();}
        catch (...) {mq = sigrok::Quantity::POWER;}

        const sigrok::Unit *unit;
        try {unit = analogPayload->unit();}
        catch (...) {unit = sigrok::Unit::DECIBEL_MW;}

        if (mq == sigrok::Quantity::POWER)
        {
            if (unit != sigrok::Unit::DECIBEL_MW)
                return;
            if (sweep_points)
                clear_trace();
            sweep_points = analogPayload->num_samples();
            values = new double[sweep_points];
            if (!values)
                sweep_points = 0;
            else
                memcpy(values, analogPayload->data_pointer(), sweep_points * sizeof(double));
        }
    }
    else if(packet->type()->id() == SR_DF_FRAME_END)
    {
        if (sweep_points == 0)
        {
            clear_trace();
            return;
        }
        double **pdata = new double*[1];
        size_t *pSweepPoints = new size_t[1];
        if (pdata && pSweepPoints)
        {
            pdata[0] = values;
            pSweepPoints[0] = sweep_points;

            SessionEvent *evt = new SessionEvent(wxEVT_SESSION_UPDATE, id_);
            evt->SetData(pdata, pSweepPoints, 1);
            wxQueueEvent(evtHandler_, evt);
        }
        else
        {
            delete [] pdata;
            delete [] pSweepPoints;
        }

        sweep_points = 0;
        values = nullptr;
    }
    else if(packet->type()->id() == SR_DF_END)
    {
        wxQueueEvent(evtHandler_, new SessionEvent(wxEVT_SESSION_ENDED, id_));
    }
}

void SpectrumFeeder::clear_trace()
{
    if (values)
        delete[] values;
    values = nullptr;
    sweep_points = 0;
}
