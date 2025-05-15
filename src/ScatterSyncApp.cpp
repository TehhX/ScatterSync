#include <ScatterSyncApp.hpp>

#include <ManifestManip.hpp>
#include <UserFileControl.hpp>

wxIMPLEMENT_APP(ScatterSyncApp);

bool ScatterSyncApp::OnInit() {
try {
    try {
        ManifestManip::readFiles();
    } catch (const ManiManiErr& mme) {
        POPUP(mme.what())
        return false;
    }

    try {
        // Must come after ManifestManip::readFiles.
        UserFileControl::init();
    } catch (const UserFileErr& ufe) {
        POPUP(ufe.what())
        return false;
    }

    mainFrame = new MainFrame;

    return true;
} catch (const ScatterSyncErr& sse) {
    POPUP(sse.what())
    return false;
}
}
