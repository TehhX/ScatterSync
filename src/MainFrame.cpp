#include <MainFrame.hpp>

#include <ScatterSyncDefs.hpp>

#define STD_GCE_POP new wxMessageDialog { this, gce.what(), wxMessageBoxCaptionStr, wxSTAY_ON_TOP | wxICON_WARNING | wxOK }
#define YN_GCE_POP new wxMessageDialog { this, gce.what(), wxMessageBoxCaptionStr, wxSTAY_ON_TOP | wxICON_WARNING | wxYES | wxNO }

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
    wxCommandEvent tempEv {};
    initEventBttn(tempEv);

    Show();
}

void MainFrame::initEventBttn(wxCommandEvent& WXUNUSED(event)) {
    try {
        gCtrl.init();
    }
    catch (const GitCtrlErr& gce) {
        auto md { STD_GCE_POP };
        md->ShowModal();
    }
}

void MainFrame::pushEventBttn(wxCommandEvent& WXUNUSED(event)) {
    try {
        gCtrl.push();
    }
    catch (const GitCtrlErr& gce) {
        auto md { STD_GCE_POP };
        md->ShowModal();
    }
}

void MainFrame::pullEventBttn(wxCommandEvent& WXUNUSED(event)) {
    try {
        gCtrl.pull();
    }
    catch (const GitCtrlErr& gce) {
        auto md { STD_GCE_POP };
        md->ShowModal();
    }
}

void MainFrame::closeWinEvent(wxCloseEvent& WXUNUSED(event)) {
    try {
        gCtrl.exitGitCtrl();
        Destroy();
    }
    catch (const GitCtrlErr& gce) {
        switch(gce.errorCode) {
        case GitCtrlErr::BAD_INIT:
            Destroy();
            break;

        case GitCtrlErr::UNPUSHED_EXIT:
            auto md { YN_GCE_POP };

            switch(md->ShowModal()) {
            case wxID_YES:
                gCtrl.exitGitCtrl(false);
                Destroy();
                break;

            case wxID_NO:
                return;
            }
        }
    }
}
