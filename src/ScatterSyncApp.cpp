#include <ScatterSyncApp.hpp>

#include <wx/frame.h>

#include <ScatterSyncDefs.hpp>

wxIMPLEMENT_APP(ScatterSyncApp);

bool ScatterSyncApp::OnInit() {
    mainFrame = new wxFrame { nullptr, wxID_ANY, "Scatter Sync" };

    mainFrame->SetClientSize(ssdef::WSX, ssdef::WSY);
    mainFrame->Center();
    mainFrame->Show();

    return true;
}
