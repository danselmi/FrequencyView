#include "FftAnalyzerOptionsDialog.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/config.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/textctrl.h>

#include <wx/msgdlg.h>

BEGIN_EVENT_TABLE(FftAnalyzerOptionsDialog, wxDialog)
    EVT_BUTTON(wxID_OK, FftAnalyzerOptionsDialog::onOK)
END_EVENT_TABLE()


FftAnalyzerOptionsDialog::FftAnalyzerOptionsDialog(wxWindow* parent, wxConfigBase *config/*, bool laDeviceCanContinuous, size_t deviceBufferSize*/):
    wxDialog(parent, wxID_ANY, "FFT/DFT Analyzer options"),
	config_(config)
{
	wxBoxSizer* BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	wxFlexGridSizer* FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);

    wxStaticText *staticText;
    staticText = new wxStaticText(this, wxID_ANY, "Parallel channels:");
    FlexGridSizer1->Add(staticText, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	spinParallel_ = new wxSpinCtrl(this, wxID_ANY);
    spinParallel_->SetToolTip("number of parallel channels in one logic sample");
    FlexGridSizer1->Add(spinParallel_, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    options_.parallelChannelsPerSample = config_->Read("/fftAnalyzerOptionsDialog/parallelChannels", 1ul);
    spinParallel_->SetRange(1, 64);
    spinParallel_->SetValue(options_.parallelChannelsPerSample);

    staticText = new wxStaticText(this, wxID_ANY, "Total channels:");
    FlexGridSizer1->Add(staticText, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	spinTotal_ = new wxSpinCtrl(this, wxID_ANY);
    spinTotal_->SetToolTip("total channels in one frame");
    FlexGridSizer1->Add(spinTotal_, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    options_.numTotalChannels = config_->Read("/fftAnalyzerOptionsDialog/totalChannels", 2ul);
    spinTotal_->SetRange(1, 64);
    spinTotal_->SetValue(options_.numTotalChannels);

    staticText = new wxStaticText(this, wxID_ANY, "MSB as IQ indicator");
    FlexGridSizer1->Add(staticText, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	chkBox_ = new wxCheckBox(this, wxID_ANY, "");
    chkBox_->SetToolTip("MSB at logic analyzer is used is channel indicator.");
    FlexGridSizer1->Add(chkBox_, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    options_.msbAsIQIndicator = config_->Read("/fftAnalyzerOptionsDialog/msbAsIQIndicator", true);
    chkBox_->SetValue(options_.msbAsIQIndicator);

	BoxSizer1->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	wxStdDialogButtonSizer* StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
}

FftAnalyzerOptionsDialog::Options FftAnalyzerOptionsDialog::getOption()
{
    return options_;
}

void FftAnalyzerOptionsDialog::onOK(wxCommandEvent &event)
{
    options_.parallelChannelsPerSample = spinParallel_->GetValue();
    options_.numTotalChannels = spinTotal_->GetValue();
    options_.msbAsIQIndicator = chkBox_->GetValue();

    config_->Write("/fftAnalyzerOptionsDialog/parallelChannels", options_.parallelChannelsPerSample);
    config_->Write("/fftAnalyzerOptionsDialog/totalChannels", options_.numTotalChannels);
    config_->Write("/fftAnalyzerOptionsDialog/msbAsIQIndicator", options_.msbAsIQIndicator);

    EndModal(wxID_OK);
}

