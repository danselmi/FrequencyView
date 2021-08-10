#ifndef CENTERFREQDIALOG_H
#define CENTERFREQDIALOG_H

//(*Headers(CenterFreqDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
//*)

class CenterFreqDialog: public wxDialog
{
	public:

		CenterFreqDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~CenterFreqDialog();

		//(*Declarations(CenterFreqDialog)
		wxButton* ButtonGHz;
		wxButton* ButtonHz;
		wxButton* ButtonKHz;
		wxButton* ButtonMHz;
		wxSpinCtrlDouble* SpinCtrlDouble1;
		//*)

	protected:

		//(*Identifiers(CenterFreqDialog)
		static const long ID_SPINCTRLDOUBLE1;
		static const long ID_BUTTON_HZ;
		static const long ID_BUTTON_KHZ;
		static const long ID_BUTTON_MHZ;
		static const long ID_BUTTON_GHZ;
		//*)

	private:

		//(*Handlers(CenterFreqDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
