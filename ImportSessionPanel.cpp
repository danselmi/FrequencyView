#include "ImportSessionPanel.h"

#include <wx/config.h>
#include <wx/log.h>
#include <wx/sizer.h>

#include <wx/plotctrl/plotctrl.h>

#include "FftAnalyzerOptionsDialog.h"
#include "ImportDataFeeder.h"
#include "LogicFeeder.h"
#include "SessionEvent.h"
#include "ImportSessionThread.h"

#include <fftw3.h>
#include <complex>

BEGIN_EVENT_TABLE(ImportSessionPanel, wxPanel)
    EVT_PLOTCTRL_CURVE_SEL_CHANGING  (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_CURVE_SEL_CHANGED   (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_MOUSE_MOTION        (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_CLICKED             (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_DOUBLECLICKED       (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_POINT_CLICKED       (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_POINT_DOUBLECLICKED (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_AREA_SEL_CREATING   (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_AREA_SEL_CHANGING   (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_AREA_SEL_CREATED    (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_VIEW_CHANGING       (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_VIEW_CHANGED        (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_CURSOR_CHANGING     (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_CURSOR_CHANGED      (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_MOUSE_FUNC_CHANGING (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)
    EVT_PLOTCTRL_MOUSE_FUNC_CHANGED  (wxID_ANY, ImportSessionPanel::onPlotCtrlEvent)

    EVT_SESSION_UPDATE               (wxID_ANY, ImportSessionPanel::onSessionUpdate)
    EVT_SESSION_ENDED                (wxID_ANY, ImportSessionPanel::onSessionEnded)

    EVT_MENU                         (wxID_ANY, ImportSessionPanel::onPlot)
    EVT_UPDATE_UI                    (wxID_ANY, ImportSessionPanel::onUpdatePlot)
END_EVENT_TABLE()

ImportSessionPanel::ImportSessionPanel(wxWindow *parent, wxEvtHandler *evtHandler, wxWindowID winid,
                    std::shared_ptr<sigrok::Session> session,
                    std::shared_ptr<sigrok::Input> input,
                    const wxString &filePath):
    wxPanel(parent, winid),
    plotCtrl_(nullptr),
    session_(session),
    evtHandler_(evtHandler)
{
    initPlotControl();

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(plotCtrl_, 1, wxEXPAND);
    SetSizerAndFit(sizer);

    initCapturing(session);

    initFeederAndStartSession(input, filePath);
}

void ImportSessionPanel::initPlotControl()
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

void ImportSessionPanel::initCapturing(std::shared_ptr<sigrok::Session> session/*, std::shared_ptr<sigrok::HardwareDevice> device*/)
{
    wxConfig cfg("WaveView");
    FftAnalyzerOptionsDialog dlg(this, &cfg);
    if (dlg.ShowModal() != wxID_OK)
        return;
    FftAnalyzerOptionsDialog::Options opt = dlg.getOption();

//    std::vector<std::shared_ptr<sigrok::Channel>> channels = device->channels();

    size_t bitsPerSample = 16;
    if (opt.msbAsIQIndicator)
        --bitsPerSample;

    maxFftSize_ = 131072;

    feederConfig_.blocksize = maxFftSize_;
    feederConfig_.bitsPerSample = bitsPerSample;
    feederConfig_.numTotalChannels = opt.numTotalChannels;
    feederConfig_.parallelChannelsPerSample = opt.parallelChannelsPerSample;
}

void ImportSessionPanel::onPlot(wxCommandEvent &event)
{
//    if (event.GetId() == ID_DRAW_SYMBOLS)     plotCtrl_->SetDrawSymbols(event.IsChecked());
//    else if (event.GetId() == ID_DRAW_LINES)  plotCtrl_->SetDrawLines(event.IsChecked());
//    else if (event.GetId() == ID_DRAW_SPLINE) plotCtrl_->SetDrawSpline(event.IsChecked());
}

void ImportSessionPanel::onUpdatePlot(wxUpdateUIEvent &event)
{
//    if (event.GetId() == ID_DRAW_SYMBOLS)     event.Check(plotCtrl_->GetDrawSymbols());
//    else if (event.GetId() == ID_DRAW_LINES)  event.Check(plotCtrl_->GetDrawLines());
//    else if (event.GetId() == ID_DRAW_SPLINE) event.Check(plotCtrl_->GetDrawSpline());
}

void ImportSessionPanel::onPlotCtrlEvent(wxPlotCtrlEvent& event)
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

void ImportSessionPanel::initFeederAndStartSession(std::shared_ptr<sigrok::Input> input, const wxString &filePath)
{
    session_->remove_datafeed_callbacks();

    LogicFeeder *logicFeeder = new LogicFeeder(evtHandler_, GetId(), feederConfig_);

    auto feed_callback = [=] (std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet)
    { logicFeeder->data_feed_callback(device, packet); };
    session_->add_datafeed_callback(feed_callback);
    auto stop_callback = [=] ()
    { delete logicFeeder; };
    session_->set_stopped_callback(stop_callback);

    ImportSessionThread *thread = new ImportSessionThread(filePath, session_, input);
    if (thread->Run() != wxTHREAD_NO_ERROR)
    {
        wxLogMessage("Can't create the thread!");
        delete thread;
        thread = nullptr;
    }
}

void ImportSessionPanel::onSessionUpdate(SessionEvent &evt)
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
        //f[i] = fc_ + fs_/N*i - fs_/2;
        f[i] = -N/2 + i;
        double a = std::abs(temp);
        p[i] = (a == 0.0) ? -240.0 : 20.0 * log10(a);
        //wxLogMessage("f[%zu] = %f, p[%zu] = %f", i, f[i], i, p[i]);
    }
    fftw_destroy_plan(fft_plan);
    fftw_free(fft_out);
    fftw_free(fft_in);

    wxPlotData *pltData = new wxPlotData(f, p, len);
    //pltData->SetBoundingRect(wxRect2DDouble(f[0], -140.0, f[len-1]-f[0], 140.0));
    if (plotCtrl_->GetCurveCount())
        plotCtrl_->DeleteCurve(0, false);
    plotCtrl_->AddCurve(pltData);

    /** add start, stop, center and sweep-pts to static text field below plot window */
}

void ImportSessionPanel::onSessionEnded(SessionEvent&)
{
    wxLogMessage("Session ended");
}

