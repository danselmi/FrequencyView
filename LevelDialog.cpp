#include "LevelDialog.h"

//(*InternalHeaders(LevelDialog)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(LevelDialog)
const long LevelDialog::ID_SPINCTRLDOUBLE = wxNewId();
//*)

BEGIN_EVENT_TABLE(LevelDialog,wxDialog)
	//(*EventTable(LevelDialog)
	//*)
END_EVENT_TABLE()

LevelDialog::LevelDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(LevelDialog)
	wxBoxSizer* BoxSizer1;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	SpinCtrl = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, 0.01, _T("ID_SPINCTRLDOUBLE"));
	SpinCtrl->SetValue(_T("0"));
	BoxSizer1->Add(SpinCtrl, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 0, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);
	//*)
}

LevelDialog::~LevelDialog()
{
	//(*Destroy(LevelDialog)
	//*)
}

