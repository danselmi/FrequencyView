#ifndef FFT_ANALYZER_OPTIONS_DIALOG_H
#define FFT_ANALYZER_OPTIONS_DIALOG_H

#include <map>
#include <memory>
#include <string>

#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

class wxConfigBase;

class FftAnalyzerOptionsDialog: public wxDialog
{
public:
    struct Options {
        uint32_t parallelChannelsPerSample;
        uint32_t numTotalChannels;
        bool msbAsIQIndicator;
    };
    FftAnalyzerOptionsDialog(wxWindow* parent, wxConfigBase *config);
    virtual ~FftAnalyzerOptionsDialog(){}
    Options getOption();

private:
    void onOK(wxCommandEvent &event);

    wxConfigBase *config_;
    wxCheckBox *chkBox_;
    wxSpinCtrl *spinParallel_;
    wxSpinCtrl *spinTotal_;

    Options options_;

    DECLARE_EVENT_TABLE()
};

#endif

