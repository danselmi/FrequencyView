#include "FrequencyViewApp.h"
#include "FrequencyViewMain.h"

IMPLEMENT_APP(FrequencyViewApp);

bool FrequencyViewApp::OnInit()
{
    FrequencyViewFrame* frame = new FrequencyViewFrame(0L, _("FrequencyView"));
    frame->Show(true);
    return true;
}

int FrequencyViewApp::OnExit()
{
    return wxApp::OnExit();
}
