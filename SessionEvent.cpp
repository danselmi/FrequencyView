#include "SessionEvent.h"

#include <libsigrokcxx/libsigrokcxx.hpp>

using namespace std;

wxDEFINE_EVENT(wxEVT_SESSION_UPDATE, SessionEvent);
wxDEFINE_EVENT(wxEVT_SESSION_ENDED, SessionEvent);

SessionEvent::SessionEvent(wxEventType commandType, int id):
    wxCommandEvent(commandType, id),
    data_(nullptr),
    len_(nullptr),
    channels_(0)
{}

SessionEvent::~SessionEvent()
{
    if (len_)
        delete[] len_;
    len_ = nullptr;

    if (data_)
        for (size_t i = 0 ; i < channels_ ; ++i)
            delete [] (data_[i]);
    delete [] data_;
    data_ = nullptr;
}

SessionEvent::SessionEvent(SessionEvent& event):
    wxCommandEvent(event),
    data_(event.data_),
    len_(event.len_),
    channels_(event.channels_)
{
    event.data_ = nullptr;
    event.len_= nullptr;
    event.channels_ = 0;
}

// Required for sending with wxPostEvent()
wxEvent* SessionEvent::Clone() const
{
    return new SessionEvent(*const_cast<SessionEvent*>(this));
}

void SessionEvent::GetData(double ***dat, size_t **length, size_t *channels)
{
    *dat = data_;
    *length = len_;
    *channels = channels_;
    data_ = nullptr;
    len_ = nullptr;
    channels_ = 0;
}

void SessionEvent::SetData(double **dat, size_t *length, size_t channels)
{
    data_ = dat;
    len_ = length;
    channels_ = channels;
}


