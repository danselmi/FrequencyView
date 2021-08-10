#ifndef DEVCONNDIALOG_H
#define DEVCONNDIALOG_H

//(*Headers(DevConnDialog)
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)
#include <libsigrokcxx/libsigrokcxx.hpp>

class wxConfigBase;

class DevConnDialog: public wxDialog
{
	public:

		DevConnDialog(wxWindow* parent, std::shared_ptr<sigrok::Context> context, wxConfigBase *config);
		virtual ~DevConnDialog(){}

		//(*Declarations(DevConnDialog)
		wxButton* ButtonScan;
		wxChoice* ChoiceDriver;
		wxChoice* ChoiceProtocol;
		wxChoice* ChoiceSerialport;
		wxComboBox* ComboBoxBaudrate;
		wxListBox* ListBoxDevice;
		wxRadioButton* RadioButtonSerialport;
		wxRadioButton* RadioButtonTcp;
		wxRadioButton* RadioButtonUsb;
		wxSpinCtrl* SpinCtrlPortnr;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxTextCtrl* TextCtrlHost;
		//*)

		std::shared_ptr<sigrok::HardwareDevice> getSelectedDevice();

	protected:

		//(*Identifiers(DevConnDialog)
		static const long ID_CHOICE_DRIVER;
		static const long ID_RADIOBUTTON_USB;
		static const long ID_RADIOBUTTON_SERIAL;
		static const long ID_CHOICE_SERIALPORT;
		static const long ID_COMBOBOX_BAUDRATE;
		static const long ID_STATICTEXT1;
		static const long ID_RADIOBUTTON_TCP;
		static const long ID_TEXTCTRL_HOST;
		static const long ID_SPINCTRL_PORTNR;
		static const long ID_STATICTEXT2;
		static const long ID_CHOICE_PROTOCOL;
		static const long ID_BUTTON_SCAN;
		static const long ID_LISTBOX_DEVICE;
		//*)

	private:

		//(*Handlers(DevConnDialog)
		void onListBoxDeviceDClick(wxCommandEvent& event);
		//*)

        std::shared_ptr<sigrok::Context> context_;
        std::vector<std::string> driverNames_;
        std::vector<std::string> serialDevices_;
        std::vector<std::shared_ptr<sigrok::HardwareDevice>> devices_;
        wxConfigBase *config_;

		void onDriverSelected(wxCommandEvent& event);
		void onOK();
		void onDriverSelected();
		void onScan(wxCommandEvent &event);
		void onUpdateOk(wxUpdateUIEvent &event);
		void onUpdateSerialParameter(wxUpdateUIEvent &event);
		void onUpdateTcpParameter(wxUpdateUIEvent &event);
		void onOK(wxCommandEvent &event);

		DECLARE_EVENT_TABLE()
};

#endif
