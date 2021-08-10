#ifndef SESSION_EVENT
#define SESSION_EVENT

#include <memory>
#include <wx/event.h>

class SessionEvent;
wxDECLARE_EVENT(wxEVT_SESSION_UPDATE, SessionEvent);
wxDECLARE_EVENT(wxEVT_SESSION_ENDED, SessionEvent);

class SessionEvent: public wxCommandEvent
{
public:
	SessionEvent(wxEventType commandType = wxEVT_SESSION_UPDATE, int id = 0);
	virtual ~SessionEvent();

	// You *must* copy here the data to be transported
	SessionEvent(SessionEvent& event);

	// Required for sending with wxPostEvent()
	wxEvent* Clone()const;

	//std::shared_ptr<sigrok::Session> GetData();
	void GetData(double ***dat, size_t **length, size_t *channels);
	//void SetData(std::shared_ptr<sigrok::Session> session);
	void SetData(double **dat, size_t *length, size_t channels);

private:
    double **data_;
    size_t *len_;
    size_t channels_;
};

typedef void (wxEvtHandler::*SessionEventFunction)(SessionEvent &);

#define SessionEventHandler(func) wxEVENT_HANDLER_CAST(SessionEventFunction, func)

// define an event table entry
#define EVT_SESSION_UPDATE(id, func) \
 	wx__DECLARE_EVT1(wxEVT_SESSION_UPDATE, id, SessionEventHandler(func))
#define EVT_SESSION_ENDED(id, func) \
 	wx__DECLARE_EVT1(wxEVT_SESSION_ENDED, id, SessionEventHandler(func))

#endif

