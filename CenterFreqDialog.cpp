#include "CenterFreqDialog.h"

//(*InternalHeaders(CenterFreqDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(CenterFreqDialog)
const long CenterFreqDialog::ID_SPINCTRLDOUBLE1 = wxNewId();
const long CenterFreqDialog::ID_BUTTON_HZ = wxNewId();
const long CenterFreqDialog::ID_BUTTON_KHZ = wxNewId();
const long CenterFreqDialog::ID_BUTTON_MHZ = wxNewId();
const long CenterFreqDialog::ID_BUTTON_GHZ = wxNewId();
//*)

BEGIN_EVENT_TABLE(CenterFreqDialog,wxDialog)
	//(*EventTable(CenterFreqDialog)
	//*)
END_EVENT_TABLE()

CenterFreqDialog::CenterFreqDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(CenterFreqDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	SpinCtrlDouble1 = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE1, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 7000, 1e+11, 9.42e+08, 0.0001, _T("ID_SPINCTRLDOUBLE1"));
	SpinCtrlDouble1->SetValue(_T("0"));
	BoxSizer3->Add(SpinCtrlDouble1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	ButtonHz = new wxButton(this, ID_BUTTON_HZ, _("Hz"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_HZ"));
	BoxSizer2->Add(ButtonHz, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonKHz = new wxButton(this, ID_BUTTON_KHZ, _("kHz"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_KHZ"));
	BoxSizer2->Add(ButtonKHz, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonMHz = new wxButton(this, ID_BUTTON_MHZ, _("MHz"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_MHZ"));
	BoxSizer2->Add(ButtonMHz, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ButtonGHz = new wxButton(this, ID_BUTTON_GHZ, _("GHz"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_GHZ"));
	BoxSizer2->Add(ButtonGHz, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 0, wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);
	//*)
}

CenterFreqDialog::~CenterFreqDialog()
{
	//(*Destroy(CenterFreqDialog)
	//*)
}

