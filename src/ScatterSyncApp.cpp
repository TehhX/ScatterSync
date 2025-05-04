#include <ScatterSyncApp.hpp>

wxIMPLEMENT_APP(ScatterSyncApp);

bool ScatterSyncApp::OnInit() {
    mainFrame = new MainFrame;

    return true;
}
