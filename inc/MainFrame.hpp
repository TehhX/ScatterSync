#pragma once

#include <FileList.hpp>
#include <GitControl.hpp>
#include <SettingsFrame.hpp>
#include <UserFileControl.hpp>
#include <Popups.hpp>

#include <wx/frame.h>
#include <wx/menu.h>

class MainFrame : public wxFrame {
    struct Settings {
        u_char scrollSpeed;
        bool autoSyncOnOpen;
        bool exitPromptUnpushed;
        bool initGitOnOpen;
    };

    GitControl gCtrl;

    wxMenuBar* menuBar;

    wxMenu* menuFile;
    wxMenu* menuEdit;
    wxMenu* menuGit;

    SettingsFrame* settingsFrame;

    FileList* fileList;

    // Gets button offset relative to passed previous button. If none/nullptr passed, use starting position
    static inline wxPoint getButtonOffset(wxButton* prev = nullptr) { return { (prev ? prev->GetPosition().x + prev->GetSize().x + 10 : 10), 15 }; }

    void closeWinEvent(wxCloseEvent&);

    void standardExit(wxCloseEvent&, bool warnUnpushed = true);

    void settEvent(wxCommandEvent&);
    inline void settEvent() { wxCommandEvent ev {}; settEvent(ev); }

    void syncEvent(wxCommandEvent&);
    inline void syncEvent() { wxCommandEvent ev {}; syncEvent(ev); }

    void initEvent(wxCommandEvent&);
    inline void initEvent() { wxCommandEvent ev {}; initEvent(ev); }

    void moveAllRepoEvent(wxCommandEvent&);

    void moveAllLocalEvent(wxCommandEvent&);

public:
    static inline Settings settings {};

    MainFrame();
};
