#ifndef FFT_SESSION_PANEL
#define FFT_SESSION_PANEL

#include <memory>

#include <wx/panel.h>

#include <libsigrokcxx/libsigrokcxx.hpp>

#include "LogicFeeder.h"

class wxPlotCtrl;
class wxPlotCtrlEvent;
class SessionEvent;

class FftSessionPanel : public wxPanel
{
public:
    FftSessionPanel(wxWindow *parent, wxEvtHandler *evtHandler, wxWindowID winid,
                    std::shared_ptr<sigrok::Session> session,
                    std::shared_ptr<sigrok::HardwareDevice> device);
    ~FftSessionPanel();
private:
    wxPlotCtrl *plotCtrl_;
    std::shared_ptr<sigrok::Session> session_;
    size_t maxFftSize_;
    bool stop_;
    double fs_;
    double fc_;
    LogicFeederConfig feederConfig_;
    wxEvtHandler *evtHandler_;

    void initPlotControl();
    void initCapturing(std::shared_ptr<sigrok::Session> session, std::shared_ptr<sigrok::HardwareDevice> device);

    void onClose(wxCloseEvent &event);
    void onPlot(wxCommandEvent &event);
    void onUpdatePlot(wxUpdateUIEvent &event);
    void onPlotCtrlEvent(wxPlotCtrlEvent &event);
    void onSessionUpdate(SessionEvent&);
    void onSessionEnded(SessionEvent&);

    void initFeederAndStartSession();

    DECLARE_EVENT_TABLE()
};

#endif

