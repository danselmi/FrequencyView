#include "DataFeeder.h"
#include "SessionEvent.h"

using namespace std;

DataFeeder::DataFeeder(wxEvtHandler *handler, SourceType sourceType):
    sourceType_(sourceType),
    sweep_points_x(0),
    sweep_points_y(0),
    sweep_points_z(0),
    x_vals(nullptr),
    y_vals(nullptr),
    z_vals(nullptr),
    evtHandler_(handler)
{}

void DataFeeder::data_feed_callback(shared_ptr<sigrok::Device> device, shared_ptr<sigrok::Packet> packet)
{
    switch (sourceType_)
    {
    case NetworkAnalyzer:
        data_feed_callback_na(device, packet);
        break;
    case AnalogTimeSeries:
        data_feed_callback_ats(device, packet);
        break;
    case DigitalTimeSeries:
        data_feed_callback_dts(device, packet);
        break;
    }
}

void DataFeeder::clear_x_trace()
{
    if (x_vals)
        delete[] x_vals;
    x_vals = nullptr;
    sweep_points_x = 0;
}

void DataFeeder::clear_y_trace()
{
    if (y_vals)
        delete[] y_vals;
    y_vals = nullptr;
    sweep_points_y = 0;
}

void DataFeeder::clear_z_trace()
{
    if (z_vals)
        delete[] z_vals;
    z_vals = nullptr;
    sweep_points_z = 0;
}
//
//void DataFeeder::data_feed_callback_sa(shared_ptr<sigrok::Device> device, shared_ptr<sigrok::Packet> packet)
//{
//    if(packet->type()->id() == SR_DF_FRAME_BEGIN)
//    {
//        sweep_points_x = 0;
//        sweep_points_y = 0;
//        sweep_points_z = 0;
//    }
//    else if(packet->type()->id() == SR_DF_ANALOG)
//    {
//        shared_ptr<sigrok::Analog> analogPayload = std::dynamic_pointer_cast<sigrok::Analog>(packet->payload());
//
//        const sigrok::Quantity *mq;
//        try {mq = analogPayload->mq();}
//        catch (...) {mq = sigrok::Quantity::VOLTAGE;}
//
//        const sigrok::Unit *unit;
//        try {unit = analogPayload->unit();}
//        catch (...) {unit = sigrok::Unit::VOLT;}
//
//        if (mq == sigrok::Quantity::FREQUENCY)
//        {
//            if (unit != sigrok::Unit::HERTZ)
//                return;
//            if (sweep_points_x)
//                clear_x_trace();
//            // check that P and f series has the same length
//            if (sweep_points_y && sweep_points_y != analogPayload->num_samples())
//                clear_y_trace();
//            sweep_points_x = analogPayload->num_samples();
//            x_vals = new double[sweep_points_x];
//            if (!x_vals)
//                sweep_points_x = 0;
//            else
//            memcpy(x_vals, analogPayload->data_pointer(), sweep_points_x * sizeof(double));
//        }
//        else if (mq == sigrok::Quantity::POWER)
//        {
//            if (unit != sigrok::Unit::DECIBEL_MW)
//                return;
//            if (sweep_points_y)
//                clear_y_trace();
//            // check that P and f series has the same length
//            if (sweep_points_x && sweep_points_x != analogPayload->num_samples())
//                clear_x_trace();
//            sweep_points_y = analogPayload->num_samples();
//            y_vals = new double[sweep_points_y];
//            if (!y_vals)
//                sweep_points_y = 0;
//            else
//                memcpy(y_vals, analogPayload->data_pointer(), sweep_points_y * sizeof(double));
//        }
//    }
//    else if(packet->type()->id() == SR_DF_FRAME_END)
//    {
//        if (sweep_points_x == 0 || sweep_points_y == 0)
//        {
//            clear_x_trace();
//            clear_y_trace();
//            return;
//        }
////        SessionEvent *evt = new SessionEvent(wxEVT_SESSION_UPDATE);
////        evt->SetData(x_vals, sweep_points_x, y_vals, sweep_points_y, z_vals, sweep_points_z);
////        wxQueueEvent(evtHandler_, evt);
//
//        sweep_points_x = 0;
//        sweep_points_y = 0;
//        x_vals = nullptr;
//        y_vals = nullptr;
//    }
//    else if(packet->type()->id() == SR_DF_END)
//    {
//        wxQueueEvent(evtHandler_, new SessionEvent(wxEVT_SESSION_ENDED));
//    }
//}

void DataFeeder::data_feed_callback_na(shared_ptr<sigrok::Device> device, shared_ptr<sigrok::Packet> packet)
{
    if(packet->type()->id() == SR_DF_FRAME_BEGIN)
    {
        sweep_points_x = 0;
        sweep_points_y = 0;
        sweep_points_z = 0;
    }
    else if(packet->type()->id() == SR_DF_ANALOG)
    {
        shared_ptr<sigrok::Analog> analogPayload = std::dynamic_pointer_cast<sigrok::Analog>(packet->payload());

        const sigrok::Quantity *mq;
        try {mq = analogPayload->mq();}
        catch (...) {mq = sigrok::Quantity::VOLTAGE;}

        const sigrok::Unit *unit;
        try {unit = analogPayload->unit();}
        catch (...) {unit = sigrok::Unit::VOLT;}

        if (mq == sigrok::Quantity::RESISTANCE)
        {
            if (unit != sigrok::Unit::OHM)
                return;
///            if (!(analogPayload->mq_flags() & sigrok::QuantityFlag::REFERENCE))
///                return;
            sweep_points_z = analogPayload->num_samples();
            z_vals = new double[sweep_points_z];
            if (!z_vals)
                sweep_points_z = 0;
            else
                memcpy(z_vals, analogPayload->data_pointer(), sweep_points_z * sizeof(double));
        }
        else if (mq == sigrok::Quantity::FREQUENCY)
        {
            if (analogPayload->unit() != sigrok::Unit::HERTZ)
                return;
            sweep_points_x = analogPayload->num_samples();
            x_vals = new double[sweep_points_x];
            if (!x_vals)
                sweep_points_x = 0;
            else
                memcpy(x_vals, analogPayload->data_pointer(), sweep_points_x * sizeof(double));
        }
#ifdef NETWORK_ANALYZER
        else if (mq == sigrok::Quantity::N_PORT_PARAMETER)
        {
            if (analogPayload->unit() != sigrok::Unit::UNITLESS)
                return;
            sweep_points_y = analogPayload->num_samples();
            y_vals = new double[sweep_points_y];
            if (!y_vals)
                sweep_points_y = 0;
            else
                memcpy(y_vals, analogPayload->data_pointer(), sweep_points_y * sizeof(double));
        }
#endif
    }
    else if(packet->type()->id() == SR_DF_FRAME_END)
    {
//        SessionEvent *evt = new SessionEvent(wxEVT_SESSION_UPDATE);
//        evt->SetData(x_vals, sweep_points_x, y_vals, sweep_points_y, z_vals, sweep_points_z);
//        wxQueueEvent(evtHandler_, evt);

        sweep_points_x = 0;
        sweep_points_y = 0;
        sweep_points_z = 0;
    }
    else if(packet->type()->id() == SR_DF_END)
    {
        wxQueueEvent(evtHandler_, new SessionEvent(wxEVT_SESSION_ENDED));
    }
}

void DataFeeder::data_feed_callback_ats(shared_ptr<sigrok::Device> device, shared_ptr<sigrok::Packet> packet)
{
    if(packet->type()->id() == SR_DF_FRAME_BEGIN)
    {
        sweep_points_y = 0;
    }
    else if(packet->type()->id() == SR_DF_ANALOG)
    {
        shared_ptr<sigrok::Analog> analogPayload = std::dynamic_pointer_cast<sigrok::Analog>(packet->payload());

        const sigrok::Quantity *mq;
        try {mq = analogPayload->mq();}
        catch (...) {mq = sigrok::Quantity::VOLTAGE;}

        const sigrok::Unit *unit;
        try {unit = analogPayload->unit();}
        catch (...) {unit = sigrok::Unit::VOLT;}

        if (mq == sigrok::Quantity::VOLTAGE)
        {
            if (unit != sigrok::Unit::VOLT)
                return;
            if (sweep_points_y)
                clear_y_trace();
            sweep_points_y = analogPayload->num_samples();
            y_vals = new float[sweep_points_y];
            if (!y_vals)
                sweep_points_y = 0;
            else
                analogPayload->get_data_as_float(y_vals);
        }
    }
    else if(packet->type()->id() == SR_DF_FRAME_END)
    {
        if (sweep_points_y)
        {
//            SessionEvent *evt = new SessionEvent(wxEVT_SESSION_UPDATE);
//            evt->SetData(x_vals, sweep_points_x, y_vals, sweep_points_y, z_vals, sweep_points_z);
//            wxQueueEvent(evtHandler_, evt);
            sweep_points_y = 0;
        }
    }
    else if(packet->type()->id() == SR_DF_END)
    {
        wxQueueEvent(evtHandler_, new SessionEvent(wxEVT_SESSION_ENDED));
    }
}

void DataFeeder::data_feed_callback_dts(shared_ptr<sigrok::Device> device, shared_ptr<sigrok::Packet> packet)
{
    if(packet->type()->id() == SR_DF_FRAME_BEGIN)
    {
        sweep_points_y = 0;
    }
    else if(packet->type()->id() == SR_DF_LOGIC)
    {
    }
    else if(packet->type()->id() == SR_DF_FRAME_END)
    {
//        SessionEvent *evt = new SessionEvent(wxEVT_SESSION_UPDATE);
//        evt->SetData(x_vals, sweep_points_x, y_vals, sweep_points_y, z_vals, sweep_points_z);
//        wxQueueEvent(evtHandler_, evt);

        sweep_points_y = 0;
    }
    else if(packet->type()->id() == SR_DF_END)
    {
        wxQueueEvent(evtHandler_, new SessionEvent(wxEVT_SESSION_ENDED));
    }
}

