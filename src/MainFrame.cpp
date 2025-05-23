#include <MainFrame.hpp>

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>
#include <GitControl.hpp>
#include <Popups.hpp>

#include <iostream>

void MainFrame::closeWinEvent(wxCloseEvent& ce) {
    try {
        try {
            standardExit(ce);
        }
        catch (const GitCtrlErr& gce)   {
            switch (gce.errCode) {
            default:
                std::cerr << "Unknown GCE error code.\n";
                throw;

            case GitCtrlErr::BAD_INIT:
                standardExit(ce, false);
                break;

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
            throw;

        case ManiManiErr::FAIL_CLOSE:
        case ManiManiErr::FAIL_WRITE:
            YN_POP(mme.what(), ce.Skip();)
        }
    }
}

void MainFrame::standardExit(wxCloseEvent& ce, bool warnUnpushed) {
    gCtrl.exitGitCtrl(warnUnpushed);
    fileList->submitAllUpdates();
    ManifestManip::writeFiles();
    ManifestManip::closeFile();
    ce.Skip();
}

void MainFrame::settEvent(wxCommandEvent& WXUNUSED(event)) {
    settingsFrame->Show();
    settingsFrame->Raise();
}

void MainFrame::syncEvent(wxCommandEvent& WXUNUSED(event)) {
    try {
        fileList->submitAllUpdates();
        ManifestManip::writeFiles();

        gCtrl.setEdited();
        gCtrl.sync();

        ManifestManip::readFiles();

        fileList->intake();
    } catch (const GitCtrlErr& gce) {
        if (gce.errCode == GitCtrlErr::SOME_OUTSIDE)
            YN_POP(gce.what(), gCtrl.sync(false);)
        else
            POPUP(gce.what())
    } catch (const ManiManiErr& mme) {
        POPUP(mme.what())
    }
}

void MainFrame::initEvent(wxCommandEvent& WXUNUSED(event)) {
    try {
        gCtrl.init();
    } catch (const GitCtrlErr& gce) {
        POPUP(gce.what())
    }
}

void MainFrame::moveAllRepoEvent(wxCommandEvent& WXUNUSED(event)) {
    try {
        fileList->submitAllUpdates();
        UserFileControl::takeActionsForEach(UserFileControl::Action::MOVE_TO_REPO);
        gCtrl.setEdited();
    } catch (const ScatterSyncErr& sse) {
        POPUP(sse.what())
    }
}

void MainFrame::moveAllLocalEvent(wxCommandEvent& WXUNUSED(event)) {
    try {
        fileList->submitAllUpdates();
        UserFileControl::takeActionsForEach(UserFileControl::Action::MOVE_TO_LOCAL);
    } catch (const ScatterSyncErr& sse) {
        POPUP(sse.what())
    }
}

MainFrame::MainFrame()
: wxFrame { nullptr, wxID_ANY, "Scatter Sync" } {
    SetClientSize(WINDOW_SIZE);
    SetBackgroundColour(WXC_DGREY);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::closeWinEvent, this);
    Center();

    fileList      = new FileList      { this };
    settingsFrame = new SettingsFrame { this };

    initBttn = new wxButton { this, wxID_ANY, "Init", getButtonOffset() };
    initBttn->Bind(wxEVT_BUTTON, &MainFrame::initEvent, this);

    syncBttn = new wxButton { this, wxID_ANY, "Sync", getButtonOffset(initBttn) };
    syncBttn->Bind(wxEVT_BUTTON, &MainFrame::syncEvent, this);

    settBttn = new wxButton { this, wxID_ANY, "Settings", getButtonOffset(syncBttn) };
    settBttn->Bind(wxEVT_BUTTON, &MainFrame::settEvent, this);

    moveRepoBttn = new wxButton { this, wxID_ANY, "Move All to Repo", getButtonOffset(settBttn) };
    moveRepoBttn->Bind(wxEVT_BUTTON, &MainFrame::moveAllRepoEvent, this);

    moveLocalBttn = new wxButton { this, wxID_ANY, "Move All to Local Paths", getButtonOffset(moveRepoBttn) };
    moveLocalBttn->Bind(wxEVT_BUTTON, &MainFrame::moveAllLocalEvent, this);

    trackNewBttn = new wxButton { this, wxID_ANY, "Track New File", getButtonOffset(moveLocalBttn) };
    trackNewBttn->Bind(wxEVT_BUTTON, &FileList::createNewFile, fileList);

    if (settings.initGitOnOpen) {
        initEvent();

        // Can't auto sync if not initialized on open
        if (settings.autoSyncOnOpen)
            syncEvent();
    }

    Show();
}
