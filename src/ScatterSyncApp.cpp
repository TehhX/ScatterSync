#include <ScatterSyncApp.hpp>

#include <wx/frame.h>

#include <ScatterSyncDefs.hpp>

wxIMPLEMENT_APP(ScatterSyncApp);

bool ScatterSyncApp::OnInit() {
    auto frame = new wxFrame { nullptr, wxID_ANY, "Scatter Sync" };

    frame->SetClientSize(ssdef::WSX, ssdef::WSY);
    frame->Center();
    frame->Show();

    return true;
}
