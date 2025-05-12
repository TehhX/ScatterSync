#include <MainFrame.hpp>

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>

MainFrame::MainFrame()
: wxFrame { nullptr, wxID_ANY, "Scatter Sync" } {
    SetClientSize(SS_GLOBALDEFS::WSX, SS_GLOBALDEFS::WSY);
    SetBackgroundColour(SS_GLOBALDEFS::DARK_GREY);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::closeWinEvent, this);
    Center();

    fileList      = new FileList      { this };
    settingsFrame = new SettingsFrame { this };

    initBttn = new wxButton { this, wxID_ANY, "Init", { 30, 20 }, { 60, -1 } };
    initBttn->Bind(wxEVT_BUTTON, &MainFrame::initEventBttn, this);

    pushBttn = new wxButton { this, wxID_ANY, "Push", { 100, 20 }, { 60, -1 } };
    pushBttn->Bind(wxEVT_BUTTON, &MainFrame::pushEventBttn, this);

    pullBttn = new wxButton { this, wxID_ANY, "Pull", { 170, 20 }, { 60, -1 } };
    pullBttn->Bind(wxEVT_BUTTON, &MainFrame::pullEventBttn, this);

    settBttn = new wxButton { this, wxID_ANY, "Settings", { 240, 20 }, { 80, -1 } };
    settBttn->Bind(wxEVT_BUTTON, &MainFrame::settEventBttn, this);
    
    wxCommandEvent emptyTemporaryEvent;
    initEventBttn(emptyTemporaryEvent);

    Show();
}

// Tries to execute GitControl::PURPOSE, e.g. init. If an error is thrown, display a simple popup describing the error.
#define GIT_BUTTON(PURPOSE) \
void MainFrame::PURPOSE##EventBttn(wxCommandEvent& WXUNUSED(event)) { \
    try { \
        gCtrl.PURPOSE(); \
    } catch (const GitCtrlErr& gce) { \
        POPUP(gce.what()) \
    } \
}

GIT_BUTTON(init)
GIT_BUTTON(push)
GIT_BUTTON(pull)

void MainFrame::settEventBttn(wxCommandEvent& WXUNUSED(event)) {
    settingsFrame->Show();
    settingsFrame->Raise();
}

void MainFrame::closeWinEvent(wxCloseEvent& ce) {
    try {
        try {
            standardExit(ce);
        }
        catch (const GitCtrlErr& gce)   {
            switch(gce.errCode) {
            default:
                std::cerr << "Unknown GCE error code.\n";
                throw gce;

            case GitCtrlErr::BAD_INIT:
                standardExit(ce, false);
                return;

            case GitCtrlErr::UNPUSHED_EXIT:
                if (MainFrame::settings.exitPromptUnpushed)
                    YN_POP(gce.what(), standardExit(ce, false);)
                else
                    standardExit(ce, false);
            }
        }
    }
    catch (const ManiManiErr& mme) {
        switch (mme.errCode) {
        default:
            std::cerr << "Unknown MME error code.\n";
            throw mme;

        case ManiManiErr::FAIL_CLOSE:
        case ManiManiErr::FAIL_WRITE:
            YN_POP(mme.what(), ce.Skip();)
        }
    }
}

void MainFrame::standardExit(wxCloseEvent& ce, bool warnUnpushed) {
    gCtrl.exitGitCtrl(warnUnpushed); \
    fileList->submitAllUpdates(); \
    ManifestManip::writeFiles(); \
    ManifestManip::closeFile(); \
    ce.Skip();
}
