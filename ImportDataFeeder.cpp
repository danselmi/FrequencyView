#include "ImportDataFeeder.h"
#include "SessionEvent.h"

using namespace std;

ImportDataFeeder::ImportDataFeeder(wxEvtHandler *handler, int id):
    evtHandler_(handler),
    id_(id)
{}

void ImportDataFeeder::data_feed_callback(shared_ptr<sigrok::Device> device, shared_ptr<sigrok::Packet> packet)
{
    if (packet->type()->id() == SR_DF_HEADER)
    {
        return;
    }
    if (packet->type()->id() == SR_DF_FRAME_BEGIN)
    {
        return;
    }
    if (packet->type()->id() == SR_DF_LOGIC)
    {
        return;
    }
    if (packet->type()->id() == SR_DF_ANALOG)
    {
        return;
    }
    if (packet->type()->id() == SR_DF_FRAME_END)
    {
        return;
    }
    if (packet->type()->id() == SR_DF_END)
    {
        //freeBuffer();
        wxQueueEvent(evtHandler_, new SessionEvent(wxEVT_SESSION_ENDED, id_));
        return;
    }
}

