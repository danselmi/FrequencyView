#include "ImportOptionsDialog.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/textctrl.h>

#include <wx/msgdlg.h>


BEGIN_EVENT_TABLE(ImportOptionsDialog,wxDialog)
END_EVENT_TABLE()

ImportOptionsDialog::ImportOptionsDialog(wxWindow* parent, wxString title, std::map<std::string, std::shared_ptr<sigrok::Option> > options):
    wxDialog(parent, wxID_ANY, title),
    options_(options)
    //ret_opts_()
{
	wxBoxSizer* BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	wxFlexGridSizer* FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);

    for (auto &itopt : options)
    {
        std::shared_ptr<sigrok::Option> opt = itopt.second;
        const Glib::VariantBase default_value = opt->default_value();
        ret_opts_[opt->id()] = default_value;

        wxStaticText *StaticText = new wxStaticText(this, wxID_ANY, opt->name().c_str());
        FlexGridSizer1->Add(StaticText, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
        int id = wxNewId();
        id_num_to_str_[id] = opt->id();
        std::vector<Glib::VariantBase> vals = opt->values();
        if (!vals.empty())
        {
            if (!default_value.gobj())
                continue;
            wxString defStr;
            int idx = 0;
            int defidx = -1;
            if (default_value.is_of_type(Glib::VariantType("s")))
                defStr = Glib::VariantBase::cast_dynamic<Glib::Variant<std::string>>(default_value).get();
            else
                defStr = default_value.print().c_str();

            wxArrayString choices;
            for (auto val : vals)
            {
                wxString str;
                if (!val.gobj())
                    str = "(NULL)";
                else if (val.is_of_type(Glib::VariantType("s")))
                    str = Glib::VariantBase::cast_dynamic<Glib::Variant<std::string>>(val).get();
                else
                    str = val.print().c_str();
                choices.push_back(str);
                if (str == defStr)
                    defidx = idx;
                idx++;
            }

            wxChoice *choice = new wxChoice(this, id, wxDefaultPosition, wxDefaultSize, choices);
            choice->SetToolTip(opt->description().c_str());
            FlexGridSizer1->Add(choice, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
            if (defidx >= 0)
                choice->SetSelection(defidx);
            choice->Connect(id, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ImportOptionsDialog::onChoice), nullptr, this);
        }
        else if (default_value.is_of_type(Glib::VariantType("b")))
        {
            wxCheckBox *chkBox = new wxCheckBox(this, id, "");
            chkBox->SetToolTip(opt->description().c_str());
            FlexGridSizer1->Add(chkBox, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
            chkBox->SetValue(
                    Glib::VariantBase::cast_dynamic<Glib::Variant<bool>>(default_value).get());
            chkBox->Connect(id, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(ImportOptionsDialog::onCheckbox), nullptr, this);
        }
        else if (default_value.is_of_type(Glib::VariantType("d")))
        {
            wxSpinCtrlDouble *spin = new wxSpinCtrlDouble(this, id);
            spin->SetToolTip(opt->description().c_str());
            FlexGridSizer1->Add(spin, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
            spin->SetValue(
                    Glib::VariantBase::cast_dynamic<Glib::Variant<double>>(default_value).get());
            spin->Connect(id, wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxCommandEventHandler(ImportOptionsDialog::onSpinCtrlDbl), nullptr, this);
        }
        else if (default_value.is_of_type(Glib::VariantType("n")) ||
			default_value.is_of_type(Glib::VariantType("q")) ||
			default_value.is_of_type(Glib::VariantType("i")) ||
			default_value.is_of_type(Glib::VariantType("u")) ||
			default_value.is_of_type(Glib::VariantType("x")) ||
			default_value.is_of_type(Glib::VariantType("t")) ||
			default_value.is_of_type(Glib::VariantType("h")))
        {
            wxSpinCtrl *spin = new wxSpinCtrl(this, id);
            spin->SetToolTip(opt->description().c_str());
            FlexGridSizer1->Add(spin, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
            if (default_value.is_of_type(Glib::VariantType("n")))
            {
                spin->SetRange(INT16_MIN, INT16_MAX);
                spin->SetValue(Glib::VariantBase::cast_dynamic<Glib::Variant<gint16>>(default_value).get());
            }
            else if (default_value.is_of_type(Glib::VariantType("q")))
            {
                spin->SetRange(0, UINT16_MAX);
                spin->SetValue(Glib::VariantBase::cast_dynamic<Glib::Variant<guint16>>(default_value).get());
            }
            else if (default_value.is_of_type(Glib::VariantType("i")) ||
                     default_value.is_of_type(Glib::VariantType("h")))
            {
                spin->SetRange(INT32_MIN, INT32_MAX);
                spin->SetValue(Glib::VariantBase::cast_dynamic<Glib::Variant<gint32>>(default_value).get());
            }
            else if (default_value.is_of_type(Glib::VariantType("u")))
            {
                spin->SetRange(0, UINT32_MAX);
                spin->SetValue(Glib::VariantBase::cast_dynamic<Glib::Variant<guint32>>(default_value).get());
            }
            else if (default_value.is_of_type(Glib::VariantType("x")))
            {
                spin->SetRange(INT64_MIN, INT64_MAX);
                spin->SetValue(Glib::VariantBase::cast_dynamic<Glib::Variant<gint64>>(default_value).get());
            }
            else if (default_value.is_of_type(Glib::VariantType("t")))
            {
                spin->SetRange(0, UINT64_MAX);
                spin->SetValue(Glib::VariantBase::cast_dynamic<Glib::Variant<guint64>>(default_value).get());
            }
            spin->Connect(id, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler(ImportOptionsDialog::onSpinCtrl), nullptr, this);
        }
		else if (default_value.is_of_type(Glib::VariantType("s")))
        {
            wxTextCtrl *txtCtrl = new wxTextCtrl(this, id, default_value.print().c_str());
            txtCtrl->SetToolTip(opt->description().c_str());
            FlexGridSizer1->Add(txtCtrl, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
            if (default_value.is_of_type(Glib::VariantType("s")))
                txtCtrl->SetValue(Glib::VariantBase::cast_dynamic<Glib::Variant<std::string>>(default_value).get());
            else
                txtCtrl->SetValue(default_value.print().c_str());
            txtCtrl->Connect(id, wxEVT_TEXT, wxCommandEventHandler(ImportOptionsDialog::onTextCtrl), nullptr, this);
        }
    }
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

std::map<std::string, Glib::VariantBase> ImportOptionsDialog::getOption()
{
    return ret_opts_;
}

void ImportOptionsDialog::onChoice(wxCommandEvent& event)
{
    wxChoice *choice = dynamic_cast<wxChoice*>(event.GetEventObject());
    if (!choice)
        return;
    int id = event.GetId();
    std::string id_str = id_num_to_str_[id];
    Glib::VariantBase val = ret_opts_[id_str];
    if (val.is_of_type(Glib::VariantType("s")))
    {
        Glib::ustring str(choice->GetString(choice->GetSelection()).c_str());
        ret_opts_[id_str] = Glib::Variant<Glib::ustring>::create(str);
    }
    else
    {
        std::string str(choice->GetString(choice->GetSelection()).c_str());
        ret_opts_[id_str] = options_[id_str]->parse_string(str);
    }
}

void ImportOptionsDialog::onCheckbox(wxCommandEvent& event)
{
    wxCheckBox *chkbox = dynamic_cast<wxCheckBox*>(event.GetEventObject());
    if (!chkbox)
        return;
    int id = event.GetId();
    std::string id_str = id_num_to_str_[id];
    Glib::VariantBase val = ret_opts_[id_str];
    if (val.is_of_type(Glib::VariantType("b")))
        ret_opts_[id_str] = Glib::Variant<bool>::create(chkbox->GetValue());
}


void ImportOptionsDialog::onTextCtrl(wxCommandEvent& event)
{
    wxTextCtrl *txtctrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
    if (!txtctrl)
        return;
    int id = event.GetId();
    std::string id_str = id_num_to_str_[id];
    Glib::VariantBase val = ret_opts_[id_str];
    if (!val.is_of_type(Glib::VariantType("s")))
        return;
    Glib::ustring str(txtctrl->GetValue().c_str());
    ret_opts_[id_str] = Glib::Variant<Glib::ustring>::create(str);
}

void ImportOptionsDialog::onSpinCtrl(wxCommandEvent& event)
{
    wxSpinCtrl *spin = dynamic_cast<wxSpinCtrl*>(event.GetEventObject());
    if (!spin)
        return;
    int id = event.GetId();
    std::string id_str = id_num_to_str_[id];
    Glib::VariantBase val = ret_opts_[id_str];
    if (val.is_of_type(Glib::VariantType("n")))
        ret_opts_[id_str] = Glib::Variant<gint16>::create(spin->GetValue());
    else if (val.is_of_type(Glib::VariantType("q")))
        ret_opts_[id_str] = Glib::Variant<guint16>::create(spin->GetValue());
    else if (val.is_of_type(Glib::VariantType("i")) || val.is_of_type(Glib::VariantType("h")))
        ret_opts_[id_str] = Glib::Variant<gint32>::create(spin->GetValue());
    else if (val.is_of_type(Glib::VariantType("u")))
        ret_opts_[id_str] = Glib::Variant<guint32>::create(spin->GetValue());
    else if (val.is_of_type(Glib::VariantType("x")))
        ret_opts_[id_str] = Glib::Variant<gint64>::create(spin->GetValue());
    else if (val.is_of_type(Glib::VariantType("t")))
        ret_opts_[id_str] = Glib::Variant<guint64>::create(spin->GetValue());
}

void ImportOptionsDialog::onSpinCtrlDbl(wxCommandEvent& event)
{
    wxSpinCtrlDouble *spin = dynamic_cast<wxSpinCtrlDouble*>(event.GetEventObject());
    if (!spin)
        return;
    int id = event.GetId();
    std::string id_str = id_num_to_str_[id];
    Glib::VariantBase val = ret_opts_[id_str];
    if (val.is_of_type(Glib::VariantType("d")))
        ret_opts_[id_str] = Glib::Variant<double>::create(spin->GetValue());
}


