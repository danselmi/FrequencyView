#include "SessionThread.h"

#include <libsigrokcxx/libsigrokcxx.hpp>

SessionThread::SessionThread(std::shared_ptr<sigrok::Session> session):
    wxThread(wxTHREAD_DETACHED),
    session_(session)
{}

wxThread::ExitCode SessionThread::Entry()
{
    session_->start();
    session_->run();
    while(session_->is_running() && !TestDestroy())
          ;

    return (ExitCode)0;
}


SessionThread::~SessionThread()
{
    if (session_.use_count() == 1)
        for (auto dev : session_->devices())
            dev->close();
}
