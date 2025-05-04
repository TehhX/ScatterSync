#include <ScatterSyncApp.hpp>

#include <ScatterSyncDefs.hpp>

wxIMPLEMENT_APP(ScatterSyncApp);

bool ScatterSyncApp::OnInit() {
    mainFrame = new MainFrame;

    return true;
}
