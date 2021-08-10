#ifndef LEVELDIALOG_H
#define LEVELDIALOG_H

//(*Headers(LevelDialog)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
//*)

class LevelDialog: public wxDialog
{
	public:

		LevelDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~LevelDialog();

		//(*Declarations(LevelDialog)
		wxSpinCtrlDouble* SpinCtrl;
		//*)

	protected:

		//(*Identifiers(LevelDialog)
		static const long ID_SPINCTRLDOUBLE;
		//*)

	private:

		//(*Handlers(LevelDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
