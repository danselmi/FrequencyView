#include "ImportSessionThread.h"
#include "SessionEvent.h"

using namespace std;

ImportSessionThread::ImportSessionThread(wxString filepath, std::shared_ptr<sigrok::Session> session, std::shared_ptr<sigrok::Input> input):
    wxThread(wxTHREAD_DETACHED),
    offsset_(0),
    file_(filepath, "rb"),
    session_(session),
    input_(input)
{
    uint8_t buf;
    input_->send(&buf, 0);
    session->add_device(input->device());

    if (file_.IsOpened())
        offsset_ = file_.Length();
}

wxThread::ExitCode ImportSessionThread::Entry()
{
    if (file_.IsOpened())
    {
        while(offsset_ && !TestDestroy())
        {
            uint8_t buf[512*1024];
            size_t len = file_.Read(buf, 512*1024);
            input_->send(buf, len);
            offsset_ -= len;
        }
        input_->end();
        return (wxThread::ExitCode)0; // success
    }
    return (wxThread::ExitCode)-1;
}
