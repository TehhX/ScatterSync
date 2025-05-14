#pragma once

#include <FileList.hpp>
#include <GitControl.hpp>
#include <SettingsFrame.hpp>
#include <UserFileControl.hpp>

// UserFileControl event functions to bind buttons to
#define UFC_EVENT_FUNC(ACTION) inline void ACTION## Event(wxCommandEvent& WXUNUSED(event) = wxCommandEvent {}) { try { UserFileControl::takeActionsAll(UserFileControl::Action::ACTION##); } catch (const UserFileErr& ufe) { POPUP(ufe.what()) }}

class MainFrame : public wxFrame {
    GitControl gCtrl;

    wxButton* initBttn;
    wxButton* syncBttn;
    wxButton* settBttn;
    wxButton* moveRepoBttn;
    wxButton* moveLocalBttn;
    wxButton* trackNewBttn;

    SettingsFrame* settingsFrame;

    FileList* fileList;

    inline wxPoint getButtonOffset(wxButton* prev = nullptr) { return { prev ? prev->GetPosition().x + prev->GetSize().x + 10 : 10, 15 }; }

    void closeWinEvent(wxCloseEvent&);

    void standardExit(wxCloseEvent&, bool warnUnpushed = true);

    void settEvent(wxCommandEvent& event = wxCommandEvent {});

    void syncEvent(wxCommandEvent& event = wxCommandEvent {});

    void initEvent(wxCommandEvent&  = wxCommandEvent {});

    UFC_EVENT_FUNC(MOVE_TO_REPO)
    UFC_EVENT_FUNC(MOVE_TO_LOCAL)

public:
    struct Settings {
        long autoSyncSeconds;
        bool autoSyncOnOpen;
        int scrollSpeed;
        bool exitPromptUnpushed;
        bool initGitOnOpen;
    };

    static inline Settings settings { 0, false, 7, true, true };

    MainFrame();
};

#undef UFC_EVENT_FUNC
