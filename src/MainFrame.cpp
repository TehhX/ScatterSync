#include <MainFrame.hpp>

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>
#include <GitControl.hpp>
#include <Popups.hpp>

#include <iostream>

enum MenuButtonID {
    MOVE_REPO = wxID_HIGHEST + 1,
    MOVE_LOCAL,
    TRACK_NEW,
    SETTINGS,
    SYNC,
    INIT
};

void MainFrame::closeWinEvent(wxCloseEvent& ce) {
    try {
        try {
            standardExit(ce);
        } catch (const GitCtrlErr& gce)   {
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
    } catch (const ManiManiErr& mme) {
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
: wxFrame { nullptr, wxID_ANY, "Scatter Sync", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER } {
    SetClientSize(WINDOW_SIZE);
    SetBackgroundColour(WXC_DGREY);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::closeWinEvent, this);
    Center();

    // Prevents MainFrame from resizing things and being annoying. Does not have any practical purpose besides that.
    new wxPanel { this, wxID_ANY, { 0, 0 }, { 0, 0 } };

// Create special panels
    settingsFrame = new SettingsFrame { this };
    fileList      = new FileList      { this };

// Create menubar and menus
    menuBar = new wxMenuBar {};
    menuBar->SetParent(this);

    menuFile = new wxMenu {};
        menuFile->Append(MOVE_REPO, "Move all to repo");
        menuFile->Append(MOVE_LOCAL, "Move all to local");
        menuFile->Append(TRACK_NEW, "Track new file");
    menuBar->Append(menuFile, "File");

    menuEdit = new wxMenu {};
        menuEdit->Append(SETTINGS, "Settings");
    menuBar->Append(menuEdit, "Edit");

    menuGit = new wxMenu {};
        menuGit->Append(SYNC, "Synchronize");
        menuGit->Append(INIT, "Initialize");
    menuBar->Append(menuGit, "Git");

    SetMenuBar(menuBar);

// Bind menu buttons
    Bind(wxEVT_MENU, &MainFrame::moveAllRepoEvent,  this,     MOVE_REPO);
    Bind(wxEVT_MENU, &MainFrame::moveAllLocalEvent, this,     MOVE_LOCAL);
    Bind(wxEVT_MENU, &FileList::createNewFile,      fileList, TRACK_NEW);
    Bind(wxEVT_MENU, &MainFrame::settEvent,         this,     SETTINGS);
    Bind(wxEVT_MENU, &MainFrame::syncEvent,         this,     SYNC);
    Bind(wxEVT_MENU, &MainFrame::initEvent,         this,     INIT);

// On-open tasks
    if (settings.initGitOnOpen) {
        initEvent();
        // Can't auto sync if not initialized on open
        if (settings.autoSyncOnOpen)
            syncEvent();
    }

    Show();
}
