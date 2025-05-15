#pragma once

#include <FileList.hpp>
#include <GitControl.hpp>
#include <SettingsFrame.hpp>
#include <UserFileControl.hpp>

// UserFileControl event functions to bind buttons to
#define UFC_EVENT_FUNC(ACTION) \
    inline void ACTION##Event(wxCommandEvent& event) { \
        try { \
            gCtrl.setEdited(); \
            fileList->submitAllUpdates(); \
            UserFileControl::takeActionsForEach(UserFileControl::Action::ACTION); \
        } catch (const UserFileErr& ufe) { \
            POPUP(ufe.what()) \
        } catch(const GitCtrlErr& gce) { \
            YN_POP(std::string { gce.what() } + std::string { " Proceed anyway?" }, \
                try { \
                    UserFileControl::takeActionsForEach(UserFileControl::Action::ACTION); \
                } catch (const UserFileErr& ufe) { \
                    POPUP(ufe.what()) \
                } \
            ) \
        } \
    }

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

    inline wxPoint getButtonOffset(wxButton* prev = nullptr) { return { (prev ? prev->GetPosition().x + prev->GetSize().x + 10 : 10), 15 }; }

    void closeWinEvent(wxCloseEvent&);

    void standardExit(wxCloseEvent&, bool warnUnpushed = true);

    void settEvent(wxCommandEvent&);
    inline void settEvent() { wxCommandEvent ev {}; settEvent(ev); }

    void syncEvent(wxCommandEvent&);
    inline void syncEvent() { wxCommandEvent ev {}; syncEvent(ev); }

    void initEvent(wxCommandEvent&);
    inline void initEvent() { wxCommandEvent ev {}; initEvent(ev); }

    UFC_EVENT_FUNC(MOVE_TO_REPO)
    UFC_EVENT_FUNC(MOVE_TO_LOCAL)

public:
    struct Settings {
        u_char scrollSpeed;
        bool autoSyncOnOpen;
        bool exitPromptUnpushed;
        bool initGitOnOpen;
    };

    static inline Settings settings {};

    MainFrame();
};

#undef UFC_EVENT_FUNC
