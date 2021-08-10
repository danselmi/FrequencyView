#ifndef IMPORT_SESSION_PANEL
#define IMPORT_SESSION_PANEL

#include <memory>

#include <wx/panel.h>

#include <libsigrokcxx/libsigrokcxx.hpp>

#include "LogicFeeder.h"

class wxPlotCtrl;
class wxPlotCtrlEvent;
class SessionEvent;

class ImportSessionPanel : public wxPanel
{
public:
    ImportSessionPanel(wxWindow *parent, wxEvtHandler *evtHandler, wxWindowID winid,
                    std::shared_ptr<sigrok::Session> session,
                    std::shared_ptr<sigrok::Input> input,
                    const wxString &filePath);
    ~ImportSessionPanel() = default;
private:
    wxPlotCtrl *plotCtrl_;
    std::shared_ptr<sigrok::Session> session_;
    ;
    size_t maxFftSize_;
    bool stop_;
    double fs_;
    double fc_;
    LogicFeederConfig feederConfig_;
    wxEvtHandler *evtHandler_;

    void initPlotControl();
    void initCapturing(std::shared_ptr<sigrok::Session> session);

    void onClose(wxCloseEvent &event);
    void onPlot(wxCommandEvent &event);
    void onUpdatePlot(wxUpdateUIEvent &event);
    void onPlotCtrlEvent(wxPlotCtrlEvent &event);
    void onSessionUpdate(SessionEvent&);
    void onSessionEnded(SessionEvent&);

    void initFeederAndStartSession(std::shared_ptr<sigrok::Input> input, const wxString &filePath);

    DECLARE_EVENT_TABLE()
};

#endif

