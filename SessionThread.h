#ifndef SESSIONTHREAD_H
#define SESSIONTHREAD_H

#include <memory>
#include <wx/thread.h>

namespace sigrok{
    class Session;
}

class SessionThread : public wxThread
{
public:
    SessionThread(std::shared_ptr<sigrok::Session> session);
private:
    ~SessionThread();
    virtual ExitCode Entry();
    std::shared_ptr<sigrok::Session> session_;
};


#endif
