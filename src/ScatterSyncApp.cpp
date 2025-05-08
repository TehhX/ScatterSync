#include <ScatterSyncApp.hpp>

#include <ManifestManip.hpp>

wxIMPLEMENT_APP(ScatterSyncApp);

bool ScatterSyncApp::OnInit() {
    ManifestManip::readFiles();

    mainFrame = new MainFrame;

    return true;
}
