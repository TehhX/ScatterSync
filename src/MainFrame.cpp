#include <MainFrame.hpp>

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>

// Displays a simple popup window with MSSG as the message and no further user input.
#define POPUP(MSSG) \
    auto mssgDialog { new wxMessageDialog { nullptr, MSSG, wxMessageBoxCaptionStr, wxSTAY_ON_TOP | wxICON_WARNING | wxOK } }; \
    mssgDialog->ShowModal();

// Displays a yes/no popup window with MSSG as the message, executes EXEC if yes, nothing if no.
#define YN_POP(MSSG, EXEC) \
    auto mssgDialog { new wxMessageDialog { nullptr, MSSG, wxMessageBoxCaptionStr, wxSTAY_ON_TOP | wxICON_WARNING | wxYES | wxNO } }; \
    if (mssgDialog->ShowModal() == wxID_YES) { \
        EXEC \
    }

MainFrame::MainFrame()
: wxFrame { nullptr, wxID_ANY, "Scatter Sync" } {
    SetClientSize(SS_GLOBALDEFS::WSX, SS_GLOBALDEFS::WSY);
    SetBackgroundColour(SS_GLOBALDEFS::DARK_GREY);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::closeWinEvent, this);
    Center();

    fileList = new FileList { this };

    initBttn = new wxButton { this, wxID_ANY, "Init", { 30, 10 }, { 60, -1 } };
    initBttn->Bind(wxEVT_BUTTON, &MainFrame::initEventBttn, this);

    pushBttn = new wxButton { this, wxID_ANY, "Push", { 100, 10 }, { 60, -1 } };
    pushBttn->Bind(wxEVT_BUTTON, &MainFrame::pushEventBttn, this);

    pullBttn = new wxButton { this, wxID_ANY, "Pull", { 170, 10 }, { 60, -1 } };
    pullBttn->Bind(wxEVT_BUTTON, &MainFrame::pullEventBttn, this);

    // A temporary command event is required to initialize Git in the same way as the init button on program start.
    wxCommandEvent tempEv;
    initEventBttn(tempEv);

    Show();
}

// Tries to execute GitControl::PURPOSE, e.g. gCtrl.init(). If an error is thrown, display a simple popup describing the error.
#define GIT_BUTTON(PURPOSE) \
void MainFrame::##PURPOSE##EventBttn(wxCommandEvent& WXUNUSED(event)) { \
    try { \
        gCtrl.PURPOSE(); \
    } catch (const GitCtrlErr& gce) { \
        POPUP(gce.what()) \
    } \
}

GIT_BUTTON(init)
GIT_BUTTON(push)
GIT_BUTTON(pull)

void MainFrame::closeWinEvent(wxCloseEvent& WXUNUSED(event)) {
    try {
        try {
            gCtrl.exitGitCtrl();
            ManifestManip::closeFile();
            Destroy();
        }
        catch (const GitCtrlErr& gce) {
            switch(gce.errCode) {
            default:
                throw gce;

            case GitCtrlErr::BAD_INIT:
                ManifestManip::closeFile();
                Destroy();
                return;

            case GitCtrlErr::UNPUSHED_EXIT:
                YN_POP(gce.what(),
                    gCtrl.exitGitCtrl(false);
                    ManifestManip::closeFile();
                    Destroy();
                    return;
                )
            }
        }
    }
    catch (const ManiManiErr& mme) {
        switch (mme.errCode) {
        default:
            throw mme;

        case ManiManiErr::FAIL_CLOSE:
        case ManiManiErr::FAIL_WRITE:
            YN_POP(mme.what(), Destroy();)
        }
    }
}
