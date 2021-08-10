#include "FrequencyViewMain.h"

#include <wx/config.h>
#include <wx/ffile.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>

#include "DevConnDialog.h"
#include "FftSessionPanel.h"
#include "ImportOptionsDialog.h"
#include "ImportSessionPanel.h"
#include "ImportSessionThread.h"
#include "SessionEvent.h"
#include "SpectrumAnalyzerPanel.h"

namespace{
    static const int ID_ABOUT        = wxNewId();
    static const int ID_CONNECT      = wxNewId();

    static const int ID_DRAW_SYMBOLS = wxNewId();
    static const int ID_DRAW_LINES   = wxNewId();
    static const int ID_DRAW_SPLINE  = wxNewId();
}

BEGIN_EVENT_TABLE(FrequencyViewFrame, wxFrame)
    EVT_MENU           (wxID_EXIT,  FrequencyViewFrame::onExit)
    EVT_MENU           (ID_ABOUT,   FrequencyViewFrame::onAbout)
    EVT_MENU           (ID_CONNECT, FrequencyViewFrame::onConnect)
    EVT_SESSION_UPDATE (wxID_ANY,   FrequencyViewFrame::onSessionEvent)
    EVT_SESSION_ENDED  (wxID_ANY,   FrequencyViewFrame::onSessionEvent)
END_EVENT_TABLE()

FrequencyViewFrame::FrequencyViewFrame(wxFrame *frame, const wxString& title):
    wxFrame(frame, -1, title, wxDefaultPosition, wxSize(1024, 768)),
    context_(sigrok::Context::create()),
    config_(new wxConfig("FrequencyView"))
{
    auiManager_.SetManagedWindow(this);

    wxMenuBar *mbar = new wxMenuBar();
    wxMenu *fileMenu = new wxMenu("");
    wxMenu *load = new wxMenu("");
    populateFileLoaderMenu(load);
    fileMenu->AppendSubMenu(load, "Import from");
    fileMenu->Append(wxID_EXIT, _("&Quit\tAlt-F4"), _("Quit the application"));

    mbar->Append(fileMenu, _("&File"));

    wxMenu *plotMenu = new wxMenu("");
    plotMenu->Append(ID_DRAW_SYMBOLS, _T("Draw curve symbols"), _T("Draw symbols for each curve data point"), true);
    plotMenu->Append(ID_DRAW_LINES,   _T("Draw curve lines"),   _T("Draw the curves as interconnecting lines"), true);
    plotMenu->Append(ID_DRAW_SPLINE,  _T("Draw curve spline"),  _T("Draw the curves as splines"), true);
    mbar->Append(plotMenu, _T("&Plot"));

    wxMenu *speciMenu = new wxMenu("");
    speciMenu->Append(ID_CONNECT, _("&Connect"), _("Connect to spectrum Analyzer"));
    mbar->Append(speciMenu, _("Analyzer"));

    wxMenu* helpMenu = new wxMenu("");
    helpMenu->Append(ID_ABOUT, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);

    CreateStatusBar(2);
    SetStatusText(_("Welcome to FrequencyView!"),0);

    logTxtCtrl_ = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                            wxDefaultPosition, wxDefaultSize,
                            wxTE_MULTILINE | wxTE_READONLY);
    wxLog* logger = new wxLogTextCtrl(logTxtCtrl_);
    wxLog::SetActiveTarget(logger);

    wxPanel* ntbkPanel = new wxPanel(this, wxID_ANY);
    auiNotebook_ = new wxAuiNotebook(ntbkPanel, wxID_ANY);
    wxBoxSizer* ntbkPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    ntbkPanelSizer->Add(auiNotebook_, 1, wxEXPAND);
    ntbkPanel->SetSizer(ntbkPanelSizer);

    auiManager_.AddPane(ntbkPanel, wxAuiPaneInfo().Center().CaptionVisible(false).CloseButton(false));
    auiManager_.AddPane(logTxtCtrl_,
        wxAuiPaneInfo().Caption("Log").Bottom().Show()
        .Floatable()
        .CloseButton(false) // TODO: could be enabled/disable from menu later
        );
    auiManager_.Update();
}

FrequencyViewFrame::~FrequencyViewFrame()
{
    auiManager_.UnInit();
    delete config_;
}

void FrequencyViewFrame::onExit(wxCommandEvent &event)
{
    Destroy();
}

void FrequencyViewFrame::onAbout(wxCommandEvent &event)
{
    wxMessageBox("FrequencyView", _("Welcome to..."));
}

void FrequencyViewFrame::initLogicAnalyzer(std::shared_ptr<sigrok::Session> session, std::shared_ptr<sigrok::HardwareDevice> device)
{
    FftSessionPanel *pnl = new FftSessionPanel(auiNotebook_, this, wxNewId(),  session, device);
    auiNotebook_->AddPage(pnl, "fft'd data from Device", true);
}

#ifdef HAS_SPECTRUM_ANALYZER
void FrequencyViewFrame::initSpectrumAnalyzer(std::shared_ptr<sigrok::Session> session, std::shared_ptr<sigrok::HardwareDevice> device)
{
    SpectrumAnalyzerPanel *pnl = new SpectrumAnalyzerPanel(auiNotebook_, this, wxNewId(),  session, device);
    auiNotebook_->AddPage(pnl, "data from Device", true);
}
#endif

#ifdef HAS_NETWORK_ANALYZER
void FrequencyViewFrame::initNetworkAnalyzer(std::shared_ptr<sigrok::Session> session, std::shared_ptr<sigrok::HardwareDevice> device)
{
    device_->config_set(sigrok::ConfigKey::SPAN,                  Glib::Variant<gdouble>::create(span));
    device_->config_set(sigrok::ConfigKey::BAND_CENTER_FREQUENCY, Glib::Variant<gdouble>::create(frequency));
}
#endif

void FrequencyViewFrame::onConnect(wxCommandEvent &event)
{
    DevConnDialog dlg(this, context_, config_);
    if (dlg.ShowModal() !=  wxID_OK)
        return;

    std::shared_ptr<sigrok::HardwareDevice> device = dlg.getSelectedDevice();
    if(!device)
        return;

    auto session = context_->create_session();
    device->open();
    session->add_device(device);

    if(session->devices().size() != 1)
    {
        wxLogMessage("No devices in session");
        return;
    }

    for (auto key : device->driver()->config_keys())
    {
        if (key == sigrok::ConfigKey::LOGIC_ANALYZER)
        {
            initLogicAnalyzer(session, device);
        }
#ifdef HAS_SPECTRUM_ANALYZER
        else if (key == sigrok::ConfigKey::SPECTRUM_ANALYZER)
        {
            initSpectrumAnalyzer(session, device);
        }
#endif
#ifdef HAS_NETWORK_ANALYZER
        else if (key == sigrok::ConfigKey::NETWORK_ANALYZER)
        {
            initNetworkAnalyzer(session, device);
        }
#endif
    }
}

void FrequencyViewFrame::populateFileLoaderMenu(wxMenu *loadMenu)
{
	for (auto& entry : context_->input_formats())
    {
        int id = wxNewId();
        loadMenu->Append(id, wxString(entry.second->description().c_str()) + " ..." );
        Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(FrequencyViewFrame::onImport));
        inputFormatCmdId_[id] = entry.first;
    }
}

wxString FrequencyViewFrame::generateWildcardString(const std::string &description, const std::vector<std::string> &exts)
{
    wxString wildcard;
    if (!exts.empty())
    {
        wildcard = description.c_str();
        wildcard += "|";
        bool first = true;
        for (auto ext : exts)
        {
            if (!first)
                wildcard += ";";
            wildcard += "*.";
            wildcard += ext.c_str();
            first = false;
        }
        wildcard += "|";
    }
    wildcard += "All Files|*.*";
    return wildcard;
}

wxString FrequencyViewFrame::getImportfileFromUser(const wxString &wildcard)
{
    wxString ret = wxEmptyString;
    wxFileDialog dlg(this, _("Import File"), config_->Read("/import/lastDir", ""),
            "", wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(dlg.ShowModal() == wxID_CANCEL)
        return wxEmptyString;

    wxString filepath = dlg.GetPath();
    if (!filepath.IsEmpty())
        config_->Write("/import/lastDir", wxFileName(filepath).GetPath());
    return filepath;
}

static bool getOptionsFromUser(const wxString &InputFormatDescription,
        std::map<std::string, std::shared_ptr<sigrok::Option>> options,
        std::map<std::string, Glib::VariantBase> &selected_opts)
{
    if (options.size())
    {
        wxString title = wxString("Import ") + InputFormatDescription + " options";
        ImportOptionsDialog imoptdlg(nullptr, title, options);
        if (imoptdlg.ShowModal() == wxID_CANCEL)
            return false;
        selected_opts = imoptdlg.getOption();
    }
    return true;
}

void FrequencyViewFrame::onImport(wxCommandEvent &event)
{
    int id = event.GetId();
    auto input_formats = context_->input_formats();

    if (inputFormatCmdId_.find(id) == inputFormatCmdId_.end())
        return;

    auto input_format = input_formats[inputFormatCmdId_[id]];

    wxString wildcard = generateWildcardString(input_format->description(),
                                               input_format->extensions());

    wxString filepath = getImportfileFromUser(wildcard);
    if (filepath.IsEmpty())
        return;

    std::map<std::string, Glib::VariantBase> sel_opts = std::map<std::string, Glib::VariantBase>();
    if (!getOptionsFromUser(input_format->description().c_str(),
            input_format->options(), sel_opts))
        return;

    std::shared_ptr<sigrok::Input> input = input_format->create_input(sel_opts);
    std::shared_ptr<sigrok::Session> session = context_->create_session();

    ImportSessionPanel *pnl = new ImportSessionPanel(auiNotebook_, this, wxNewId(), session, input, filepath);
    auiNotebook_->AddPage(pnl, "data from import", true);
}

void FrequencyViewFrame::onSessionEvent(SessionEvent &event)
{
    wxWindow *wnd = FindWindowById(event.GetId());
    if (wnd)
        wnd->GetEventHandler()->AddPendingEvent(event);
}


