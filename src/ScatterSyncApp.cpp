#include <ScatterSyncApp.hpp>

#include <ManifestManip.hpp>

wxIMPLEMENT_APP(ScatterSyncApp);

bool ScatterSyncApp::OnInit() {
    try {
        ManifestManip::readFiles();
    } catch (const ManiManiErr& mme) {
        POPUP(mme.what())
        return false;
    }

    mainFrame = new MainFrame;

    return true;
}
