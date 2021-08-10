#ifndef IMPORTOPTIONSDIALOG_H
#define IMPORTOPTIONSDIALOG_H

#include <map>
#include <memory>
#include <string>

#include <libsigrokcxx/libsigrokcxx.hpp>

#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

namespace sigrok {
    class Option;
}

class ImportOptionsDialog: public wxDialog
{
public:
    ImportOptionsDialog(wxWindow* parent, wxString title, std::map<std::string, std::shared_ptr<sigrok::Option> > options);
    virtual ~ImportOptionsDialog(){}
    std::map<std::string, Glib::VariantBase> getOption();

private:
    std::map<int, std::string> id_num_to_str_;
    std::map<std::string, std::shared_ptr<sigrok::Option>> options_;
    std::map<std::string, Glib::VariantBase> ret_opts_;
    void onChoice(wxCommandEvent& event);
    void onCheckbox(wxCommandEvent& event);
    void onTextCtrl(wxCommandEvent& event);
    void onSpinCtrl(wxCommandEvent& event);
    void onSpinCtrlDbl(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};

#endif
