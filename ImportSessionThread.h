#ifndef IMPORT_SESSION_THREAD
#define IMPORT_SESSION_THREAD

#include <wx/thread.h>
#include <wx/ffile.h>

#include <libsigrokcxx/libsigrokcxx.hpp>

class ImportSessionThread : public wxThread
{
public:
    ImportSessionThread(wxString filepath,
        std::shared_ptr<sigrok::Session> session,
        std::shared_ptr<sigrok::Input> input);

    ~ImportSessionThread() = default;

protected:
    virtual ExitCode Entry();

private:
    wxFileOffset offsset_;
    wxFFile file_;
    std::shared_ptr<sigrok::Session> session_;
    std::shared_ptr<sigrok::Input> input_;

};

#endif

