#include "FftSessionPanel.h"

#include <wx/config.h>
#include <wx/log.h>
#include <wx/sizer.h>

#include <wx/plotctrl/plotctrl.h>

#include "FftAnalyzerOptionsDialog.h"
#include "LogicFeeder.h"
#include "SessionEvent.h"
#include "SessionThread.h"

#include <fftw3.h>
#include <complex>

BEGIN_EVENT_TABLE(FftSessionPanel, wxPanel)
    EVT_PLOTCTRL_CURVE_SEL_CHANGING  (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_CURVE_SEL_CHANGED   (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_MOUSE_MOTION        (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_CLICKED             (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_DOUBLECLICKED       (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_POINT_CLICKED       (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_POINT_DOUBLECLICKED (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_AREA_SEL_CREATING   (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_AREA_SEL_CHANGING   (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_AREA_SEL_CREATED    (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_VIEW_CHANGING       (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_VIEW_CHANGED        (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_CURSOR_CHANGING     (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_CURSOR_CHANGED      (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_MOUSE_FUNC_CHANGING (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_MOUSE_FUNC_CHANGED  (wxID_ANY, FftSessionPanel::onPlotCtrlEvent)

    EVT_SESSION_UPDATE               (wxID_ANY, FftSessionPanel::onSessionUpdate)
    EVT_SESSION_ENDED                (wxID_ANY, FftSessionPanel::onSessionEnded)

    EVT_MENU                         (wxID_ANY, FftSessionPanel::onPlot)
    EVT_UPDATE_UI                    (wxID_ANY, FftSessionPanel::onUpdatePlot)
END_EVENT_TABLE()

FftSessionPanel::FftSessionPanel(wxWindow *parent, wxEvtHandler *evtHandler, wxWindowID winid, std::shared_ptr<sigrok::Session> session, std::shared_ptr<sigrok::HardwareDevice> device):
    wxPanel(parent, winid),
    plotCtrl_(nullptr),
    session_(session),
    stop_(false),
    fs_(245.76e6),
    fc_(0.0),
    evtHandler_(evtHandler)
{
    initPlotControl();

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(plotCtrl_, 1, wxEXPAND);
    SetSizerAndFit(sizer);

    initCapturing(session, device);

    initFeederAndStartSession();
}

void FftSessionPanel::initPlotControl()
{
    plotCtrl_ = new wxPlotCtrl(this, wxID_ANY);

    plotCtrl_->SetBottomAxisLabel("f [Hz]");
    plotCtrl_->SetLeftAxisLabel("a [dBfs]");
    plotCtrl_->SetShowBottomAxis(true);
    plotCtrl_->SetShowLeftAxis(true);
    plotCtrl_->SetShowBottomAxisLabel(true);
    plotCtrl_->SetShowLeftAxisLabel(true);
    plotCtrl_->SetShowKey(false);
    plotCtrl_->SetShowPlotTitle(false);
    plotCtrl_->SetDrawGrid();
}

void FftSessionPanel::initCapturing(std::shared_ptr<sigrok::Session> session, std::shared_ptr<sigrok::HardwareDevice> device)
{
    bool deviceCanContinuous = false;
    for (auto key : device->driver()->config_keys())
        if (key == sigrok::ConfigKey::CONTINUOUS)
        {
            deviceCanContinuous = true;
            break;
        }

    // set the capture ratio to 0 to have the trigger on the very first sample
    const uint64_t capture_ratio = 0;
    device->config_set(sigrok::ConfigKey::CAPTURE_RATIO, Glib::Variant<uint64_t>::create(capture_ratio));

    std::vector<std::shared_ptr<sigrok::Channel>> channels = device->channels();

    size_t bitsPerSample = channels.size();
    uint64_t sizeOfRingbuffer = static_cast<uint32_t>(Glib::VariantBase::cast_dynamic<Glib::Variant<guint64>>(device->config_get(sigrok::ConfigKey::LIMIT_SAMPLES)).get());

    wxConfig cfg("WaveView");
    FftAnalyzerOptionsDialog dlg(this, &cfg);
    if (dlg.ShowModal() != wxID_OK)
        return;
    FftAnalyzerOptionsDialog::Options opt = dlg.getOption();

    std::shared_ptr<sigrok::Trigger> trigger = session_->context()->create_trigger("Trigger_LA");
    std::shared_ptr<sigrok::TriggerStage> triggerstage = trigger->add_stage();
    if (opt.msbAsIQIndicator)
    {
        --bitsPerSample;
        // MSB is the channel bit of the streaming protocol
        triggerstage->add_match(channels[bitsPerSample], sigrok::TriggerMatchType::ZERO);
        session->set_trigger(trigger);
    }

    maxFftSize_ = 131072;
    if(!deviceCanContinuous)
        maxFftSize_ = sizeOfRingbuffer * opt.parallelChannelsPerSample / opt.numTotalChannels;

    feederConfig_.blocksize = maxFftSize_;
    feederConfig_.bitsPerSample = bitsPerSample;
    feederConfig_.numTotalChannels = opt.numTotalChannels;
    feederConfig_.parallelChannelsPerSample = opt.parallelChannelsPerSample;
}

FftSessionPanel::~FftSessionPanel()
{
    if (session_ && session_.use_count() == 1)
        for (auto dev : session_->devices())
            dev->close();
}

void FftSessionPanel::onPlot(wxCommandEvent &event)
{
//    if (event.GetId() == ID_DRAW_SYMBOLS)     plotCtrl_->SetDrawSymbols(event.IsChecked());
//    else if (event.GetId() == ID_DRAW_LINES)  plotCtrl_->SetDrawLines(event.IsChecked());
//    else if (event.GetId() == ID_DRAW_SPLINE) plotCtrl_->SetDrawSpline(event.IsChecked());
}

void FftSessionPanel::onUpdatePlot(wxUpdateUIEvent &event)
{
//    if (event.GetId() == ID_DRAW_SYMBOLS)     event.Check(plotCtrl_->GetDrawSymbols());
//    else if (event.GetId() == ID_DRAW_LINES)  event.Check(plotCtrl_->GetDrawLines());
//    else if (event.GetId() == ID_DRAW_SPLINE) event.Check(plotCtrl_->GetDrawSpline());
}

void FftSessionPanel::onPlotCtrlEvent(wxPlotCtrlEvent& event)
{
    event.Skip();
    if (!plotCtrl_)
        return; // wait until the pointer is set
    wxEventType eventType = event.GetEventType();
    int active_index = plotCtrl_->GetActiveIndex();

    if (eventType == wxEVT_PLOTCTRL_ERROR)
    {
        //SetStatusText(event.GetString());
    }
    else if (eventType == wxEVT_PLOTCTRL_MOUSE_MOTION)
    {
        wxString s = wxString::Format(wxT("Mouse (%g %g) Active index %d, "),
                                      event.GetX(), event.GetY(), active_index);
        if (plotCtrl_->IsCursorValid())
        {
            wxPoint2DDouble cursorPt = plotCtrl_->GetCursorPoint();
            s += wxString::Format(wxT("Cursor curve %d, data index %d, point (%g %g)"),
                plotCtrl_->GetCursorCurveIndex(), plotCtrl_->GetCursorDataIndex(),
                cursorPt.m_x, cursorPt.m_y);
        }
        //SetStatusText(s);
    }
    else if (eventType == wxEVT_PLOTCTRL_VIEW_CHANGING)
    {
        //SetStatusText(wxString::Format(wxT("View Changing %g %g"), event.GetX(), event.GetY()));
    }
    else if (eventType == wxEVT_PLOTCTRL_VIEW_CHANGED)
    {
        //SetStatusText(wxString::Format(wxT("View Changed %g %g"), event.GetX(), event.GetY()));
    }
    else
    {
         wxLogMessage(wxString::Format(wxT("%s xy(%g %g) CurveIndex %d, DataIndex %d, MouseFn %d\n"),
            "wxPlotCtrlEvent::GetEventName(eventType).c_str()",
            event.GetX(), event.GetY(), event.GetCurveIndex(),
            event.GetCurveDataIndex(), event.GetMouseFunction()));
    }
}

void FftSessionPanel::initFeederAndStartSession()
{
    session_->remove_datafeed_callbacks();

    LogicFeeder *logicFeeder = new LogicFeeder(evtHandler_, GetId(), feederConfig_);

    auto feed_callback = [=] (std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet)
    { logicFeeder->data_feed_callback(device, packet); };
    session_->add_datafeed_callback(feed_callback);
    auto stop_callback = [=] ()
    { delete logicFeeder; };
    session_->set_stopped_callback(stop_callback);

    if ((new SessionThread(session_))->Run() != wxTHREAD_NO_ERROR)
        wxLogMessage("Can't create the thread!");
}

void FftSessionPanel::onSessionUpdate(SessionEvent &evt)
{
    size_t channels;
    size_t *lengths;
    double **data;
    evt.GetData(&data, &lengths, &channels);
    if (channels > 1 && lengths[0] != lengths[1])
    {
        delete [] lengths;
        if (data)
            for (size_t i = 0 ; i < channels; ++i)
                delete [] (data[i]);
        delete [] data;
        return;
    }
    size_t len = lengths[0];

    fftw_complex *fft_in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*len);
    fftw_complex *fft_out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*len);
    fftw_plan fft_plan = fftw_plan_dft_1d(len, fft_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);
    for (size_t i= 0; i < len; ++i)
    {
        fft_in[i][0] =                         data[0][i];
        fft_in[i][1] = (channels == 1) ? 0.0 : data[1][i];
    }
    fftw_execute(fft_plan);
    double *f = data[0];
    double *p = (channels == 1) ? new double[len] : data[1];
    for (size_t i = 0 ; i < len; ++i)
    {
        double N = len;
        std::complex<double> temp(fft_out[i][0], fft_out[i][1]);
        temp /= N;
        f[i] = fc_ + fs_/N*i - fs_/2;
        double a = std::abs(temp);
        p[i] = (a == 0.0) ? -240.0 : 20.0 * log10(a);
    }
    fftw_destroy_plan(fft_plan);
    fftw_free(fft_out);
    fftw_free(fft_in);

    wxPlotData *pltData = new wxPlotData(f, p, len);
    pltData->SetBoundingRect(wxRect2DDouble(f[0], -140.0, f[len-1]-f[0], 140.0));
    if (plotCtrl_->GetCurveCount())
        plotCtrl_->DeleteCurve(0, false);
    plotCtrl_->AddCurve(pltData);

    /** add start, stop, center and sweep-pts to static text field below plot window */
}

void FftSessionPanel::onSessionEnded(SessionEvent&)
{
    wxLogMessage("Session ended");

    if (!stop_)
        initFeederAndStartSession();
    else
    {
        if(session_)
        {
            for (auto dev : session_->devices())
                dev->close();
            session_ = nullptr;
        }
    }
}

