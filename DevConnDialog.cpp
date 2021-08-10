#include "DevConnDialog.h"

#include <wx/msgdlg.h>
#include <wx/config.h>
//(*InternalHeaders(DevConnDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(DevConnDialog)
const long DevConnDialog::ID_CHOICE_DRIVER = wxNewId();
const long DevConnDialog::ID_RADIOBUTTON_USB = wxNewId();
const long DevConnDialog::ID_RADIOBUTTON_SERIAL = wxNewId();
const long DevConnDialog::ID_CHOICE_SERIALPORT = wxNewId();
const long DevConnDialog::ID_COMBOBOX_BAUDRATE = wxNewId();
const long DevConnDialog::ID_STATICTEXT1 = wxNewId();
const long DevConnDialog::ID_RADIOBUTTON_TCP = wxNewId();
const long DevConnDialog::ID_TEXTCTRL_HOST = wxNewId();
const long DevConnDialog::ID_SPINCTRL_PORTNR = wxNewId();
const long DevConnDialog::ID_STATICTEXT2 = wxNewId();
const long DevConnDialog::ID_CHOICE_PROTOCOL = wxNewId();
const long DevConnDialog::ID_BUTTON_SCAN = wxNewId();
const long DevConnDialog::ID_LISTBOX_DEVICE = wxNewId();
//*)

BEGIN_EVENT_TABLE(DevConnDialog, wxDialog)
	//(*EventTable(DevConnDialog)
	//*)
	EVT_CHOICE(ID_CHOICE_DRIVER,        DevConnDialog::onDriverSelected)
	EVT_BUTTON(ID_BUTTON_SCAN,          DevConnDialog::onScan)
    EVT_UPDATE_UI(ID_CHOICE_SERIALPORT, DevConnDialog::onUpdateSerialParameter)
    EVT_UPDATE_UI(ID_COMBOBOX_BAUDRATE, DevConnDialog::onUpdateSerialParameter)
    EVT_UPDATE_UI(ID_TEXTCTRL_HOST,     DevConnDialog::onUpdateTcpParameter)
    EVT_UPDATE_UI(ID_SPINCTRL_PORTNR,   DevConnDialog::onUpdateTcpParameter)
    EVT_UPDATE_UI(ID_CHOICE_PROTOCOL,   DevConnDialog::onUpdateTcpParameter)
    EVT_UPDATE_UI(wxID_OK,              DevConnDialog::onUpdateOk)

    EVT_BUTTON(wxID_OK,                 DevConnDialog::onOK)
END_EVENT_TABLE()

DevConnDialog::DevConnDialog(wxWindow* parent, std::shared_ptr<sigrok::Context> context, wxConfigBase *config):
	context_(context),
	config_(config)
{
	//(*Initialize(DevConnDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, wxID_ANY, _("Connect to Device"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Choose the driver"));
	ChoiceDriver = new wxChoice(this, ID_CHOICE_DRIVER, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_DRIVER"));
	StaticBoxSizer1->Add(ChoiceDriver, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Choose the interface"));
	RadioButtonUsb = new wxRadioButton(this, ID_RADIOBUTTON_USB, _("USB"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON_USB"));
	StaticBoxSizer2->Add(RadioButtonUsb, 0, wxALL|wxALIGN_LEFT, 5);
	RadioButtonSerialport = new wxRadioButton(this, ID_RADIOBUTTON_SERIAL, _("Serial Port"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON_SERIAL"));
	StaticBoxSizer2->Add(RadioButtonSerialport, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	ChoiceSerialport = new wxChoice(this, ID_CHOICE_SERIALPORT, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_SERIALPORT"));
	BoxSizer2->Add(ChoiceSerialport, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ComboBoxBaudrate = new wxComboBox(this, ID_COMBOBOX_BAUDRATE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX_BAUDRATE"));
	ComboBoxBaudrate->Append(_("921600"));
	ComboBoxBaudrate->Append(_("112500"));
	ComboBoxBaudrate->Append(_("57600"));
	ComboBoxBaudrate->Append(_("19200"));
	ComboBoxBaudrate->Append(_("9600"));
	BoxSizer2->Add(ComboBoxBaudrate, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("baud"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	RadioButtonTcp = new wxRadioButton(this, ID_RADIOBUTTON_TCP, _("TCP/IP"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON_TCP"));
	StaticBoxSizer2->Add(RadioButtonTcp, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	TextCtrlHost = new wxTextCtrl(this, ID_TEXTCTRL_HOST, _("127.0.0.1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL_HOST"));
	BoxSizer3->Add(TextCtrlHost, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrlPortnr = new wxSpinCtrl(this, ID_SPINCTRL_PORTNR, _T("4242"), wxDefaultPosition, wxDefaultSize, 0, 0, 65535, 4242, _T("ID_SPINCTRL_PORTNR"));
	SpinCtrlPortnr->SetValue(_T("4242"));
	BoxSizer3->Add(SpinCtrlPortnr, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("protocol"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer3->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	ChoiceProtocol = new wxChoice(this, ID_CHOICE_PROTOCOL, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_PROTOCOL"));
	ChoiceProtocol->SetSelection( ChoiceProtocol->Append(_("Raw-TCP")) );
	ChoiceProtocol->Append(_("VXI"));
	BoxSizer3->Add(ChoiceProtocol, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_LEFT, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Scan for devices"));
	ButtonScan = new wxButton(this, ID_BUTTON_SCAN, _("Scan for devices using driver above"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_SCAN"));
	StaticBoxSizer3->Add(ButtonScan, 1, wxALL|wxALIGN_TOP, 5);
	BoxSizer1->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 5);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Select the device"));
	ListBoxDevice = new wxListBox(this, ID_LISTBOX_DEVICE, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX_DEVICE"));
	StaticBoxSizer4->Add(ListBoxDevice, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(StaticBoxSizer4, 0, wxALL|wxEXPAND, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 0, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_LISTBOX_DEVICE,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&DevConnDialog::onListBoxDeviceDClick);
	//*)

	wxString lastHost;
	int lastPort;
	int lastProtocolIdx;
	int lastBaudrate;
	int lastSerialdeviceIdx;
	int lastDrvIdx;
	int lastChannel;

    lastHost = config_->Read("/ConnDialog/LastHost", wxString("127.0.0.1"));
    lastPort = config_->Read("/ConnDialog/LastPort", 4242l);
    lastProtocolIdx = config_->Read("/ConnDialog/LastProtocolIdx", 1l);
    lastSerialdeviceIdx = config_->Read("/ConnDialog/LastSerialdeviceIdx", 0l);
    lastBaudrate = config_->Read("/ConnDialog/LastBaudrate", 115200l);
    lastDrvIdx = config_->Read("/ConnDialog/LastDriver", 0l);
    lastChannel = config_->Read("/ConnDialog/LastChannel", 0l);

    std::map<std::string, std::shared_ptr<sigrok::Driver>>  driversMap = context_->drivers();

    driverNames_.clear();;
    wxArrayString displayDriverNames;
    for (auto driverEntry : driversMap)
    {
        std::shared_ptr<sigrok::Driver> driver = driverEntry.second;
        std::set<const sigrok::ConfigKey *> configKeys = driver->config_keys();

        for (auto key : configKeys)
        {
            if (key == sigrok::ConfigKey::LOGIC_ANALYZER
#ifdef HAS_SPECTRUM_ANALYZER
                || key == sigrok::ConfigKey::SPECTRUM_ANALYZER
#endif
#ifdef HAS_NETWORK_ANALYZER
                || key == sigrok::ConfigKey::NETWORK_ANALYZER
#endif
            ){
                driverNames_.push_back(driverEntry.first);
                std::shared_ptr<sigrok::Driver> driver = driverEntry.second;
                displayDriverNames.Add(wxString::Format("%s (%s)", driver->long_name().c_str(), driverEntry.first.c_str()));
            }
        }
    }
    ChoiceDriver->Set(displayDriverNames);
    if (driverNames_.size())
    {
        if ((size_t)lastDrvIdx < driverNames_.size())
            ChoiceDriver->SetSelection(lastDrvIdx);
        else
            ChoiceDriver->SetSelection(0);
        onDriverSelected();
	}

    TextCtrlHost->SetValue(lastHost);
    SpinCtrlPortnr->SetValue(lastPort);
    ChoiceProtocol->SetSelection(lastProtocolIdx);
    if (lastSerialdeviceIdx > 0 && (size_t)lastSerialdeviceIdx < serialDevices_.size())
        ChoiceSerialport->SetSelection(lastSerialdeviceIdx);

    wxString br = wxString::Format("%d", lastBaudrate);
    int pos = ComboBoxBaudrate->FindString(br);
    if (pos != wxNOT_FOUND)
    {
        ComboBoxBaudrate->SetSelection(pos);
    }
    else
    {
        ComboBoxBaudrate->Append(br);
        ComboBoxBaudrate->SetSelection(ComboBoxBaudrate->GetCount()-1);
    }

    switch (lastChannel)
    {
    case 0: RadioButtonUsb->SetValue(true); break;
    case 1: RadioButtonSerialport->SetValue(true);break;
    default:
    case 2: RadioButtonTcp->SetValue(true);break;
    }

}

void DevConnDialog::onDriverSelected(wxCommandEvent& event)
{
	onDriverSelected();
}

void DevConnDialog::onDriverSelected()
{
	serialDevices_.clear();

	int drvSel = ChoiceDriver->GetSelection();
	if (drvSel == wxNOT_FOUND || drvSel <0 || (unsigned)drvSel >= driverNames_.size())
	{
		ChoiceSerialport->Clear();
		return;
	}

	std::shared_ptr<sigrok::Driver> driver = context_->drivers()[driverNames_[drvSel]];

	serialDevices_.clear();
	wxArrayString displaySerialNames;
	for (auto& serial : context_->serials(driver))
	{
		serialDevices_.push_back(serial.first);
		displaySerialNames.Add(wxString::Format("%s (%s)", serial.first.c_str(), serial.second.c_str()));
	}
	ChoiceSerialport->Set(displaySerialNames);
	if (displaySerialNames.size())
		ChoiceSerialport->SetSelection(0);
}

void DevConnDialog::onScan(wxCommandEvent& event)
{
	ListBoxDevice->Clear();

	int drvSel = ChoiceDriver->GetSelection();
	if (drvSel == wxNOT_FOUND || drvSel < 0 || (size_t)drvSel >= driverNames_.size())
		return;

	std::shared_ptr<sigrok::Driver> driver = context_->drivers()[driverNames_[drvSel]];

	std::map<const sigrok::ConfigKey *, Glib::VariantBase> drvopts;
	if (RadioButtonSerialport->GetValue())
	{
		int serSel = ChoiceSerialport->GetSelection();
		if (serSel != wxNOT_FOUND && serSel >= 0 && (size_t)serSel < serialDevices_.size())
		{
			drvopts[sigrok::ConfigKey::CONN] = Glib::Variant<Glib::ustring>::create(serialDevices_[serSel]);
			int bdrSel;
			if ((bdrSel = ComboBoxBaudrate->GetSelection())  != wxNOT_FOUND)
				drvopts[sigrok::ConfigKey::SERIALCOMM] = Glib::Variant<Glib::ustring>::create(
					Glib::ustring(ComboBoxBaudrate->GetString(bdrSel).utf8_str())
				);
		}
	}

	if ((RadioButtonTcp->GetValue()))
	{
		wxString host = TextCtrlHost->GetLineText(0);
		if(!host.IsEmpty())
		{
			wxString conn = ChoiceProtocol->GetSelection() == 0 ? "tcp-raw/" : "vxi/";
			conn += host + wxString::Format("/%d", SpinCtrlPortnr->GetValue());
			drvopts[sigrok::ConfigKey::CONN] = Glib::Variant<Glib::ustring>::create(Glib::ustring(
				conn.utf8_str()
			));
		}
	}

	devices_.clear();
	try
	{
		devices_ = driver->scan(drvopts);

	}
	catch (const sigrok::Error &e) {
		wxMessageBox(wxString::Format("Error when scanning device driver '%s': %2", driver->name(), e.what() ));
	}

	wxArrayString displayDevices;
	for (const std::shared_ptr<sigrok::HardwareDevice>& device : devices_)
	{
		wxString text = "";
		if (device->vendor().length())
			text += device->vendor();
		if (device->model().length())
			text += " " + device->model();
		if (device->version().length())
			text += " " + device->version();
		if (device->serial_number().length())
			text += " " + device->serial_number();
		text += wxString::Format(" with %zu channels", device->channels().size());

		displayDevices.Add(text);
	}
	if (displayDevices.size() > 0)
		ListBoxDevice->Set(displayDevices);
}

void DevConnDialog::onUpdateSerialParameter(wxUpdateUIEvent &event)
{
	event.Enable(RadioButtonSerialport->GetValue());
}

void DevConnDialog::onUpdateTcpParameter(wxUpdateUIEvent &event)
{
	event.Enable(RadioButtonTcp->GetValue());
}

void DevConnDialog::onUpdateOk(wxUpdateUIEvent &event)
{
	int sel = ListBoxDevice->GetSelection();
	event.Enable(sel != wxNOT_FOUND && sel >= 0 && (size_t)sel <= devices_.size());
}

std::shared_ptr<sigrok::HardwareDevice> DevConnDialog::getSelectedDevice()
{
	int sel = ListBoxDevice->GetSelection();
	if (sel != wxNOT_FOUND && sel >= 0 && (size_t)sel <= devices_.size())
		return devices_[sel];

	return nullptr;
}

void DevConnDialog::onOK(wxCommandEvent &)
{
    onOK();
}

void DevConnDialog::onOK()
{
    int lastChannel = 2;
    if (RadioButtonUsb->GetValue())
        lastChannel = 0;
    else if (RadioButtonSerialport->GetValue())
        lastChannel = 1;

    config_->Write("/ConnDialog/LastHost", TextCtrlHost->GetValue());
    config_->Write("/ConnDialog/LastPort", SpinCtrlPortnr->GetValue());
    config_->Write("/ConnDialog/LastProtocolIdx", ChoiceProtocol->GetSelection());
    config_->Write("/ConnDialog/LastSerialdeviceIdx", ChoiceSerialport->GetSelection());
    config_->Write("/ConnDialog/LastDriver", (size_t)ChoiceDriver->GetSelection());
    config_->Write("/ConnDialog/LastChannel", lastChannel);

    int pos = ComboBoxBaudrate->GetSelection();
    if (pos != wxNOT_FOUND)
    {
        wxString brStr = ComboBoxBaudrate->GetString(pos);
        long br;
        if (brStr.ToLong(&br))
            config_->Write("/ConnDialog/LastBaudrate", (int)br);
    }
    EndModal(wxID_OK);
}

void DevConnDialog::onListBoxDeviceDClick(wxCommandEvent& event)
{
	int sel = ListBoxDevice->GetSelection();
	if (sel != wxNOT_FOUND && sel >= 0 && (size_t)sel <= devices_.size())
        onOK();
}
