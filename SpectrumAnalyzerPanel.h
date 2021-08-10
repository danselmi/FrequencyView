#ifndef SESSION_PANEL
#define SESSION_PANEL

#include <memory>

#include <wx/panel.h>

#include <libsigrokcxx/libsigrokcxx.hpp>

class wxPlotCtrl;
class wxPlotCtrlEvent;
class SessionEvent;

class SpectrumAnalyzerPanel : public wxPanel
{
public:
    SpectrumAnalyzerPanel(wxWindow *parent, wxEvtHandler *evtHandler, wxWindowID winid,
                    std::shared_ptr<sigrok::Session> session,
                    std::shared_ptr<sigrok::HardwareDevice> device);
    ~SpectrumAnalyzerPanel();
private:
    wxPlotCtrl *plotCtrl_;
    std::shared_ptr<sigrok::Session> session_;
    wxEvtHandler *evtHandler_;
    double frequency_;
    double span_;
    uint64_t rbw_;
    double ref_level_;

    void initPlotControl();

    void OnPlot(wxCommandEvent &event);
    void OnUpdatePlot(wxUpdateUIEvent &event);
    void OnPlotCtrlEvent(wxPlotCtrlEvent &event);

    void OnSessionUpdate(SessionEvent&);
    void OnSessionEnded(SessionEvent&);

    DECLARE_EVENT_TABLE()
};

#endif

