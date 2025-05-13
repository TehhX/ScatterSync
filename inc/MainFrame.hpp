#pragma once

#include <FileList.hpp>
#include <GitControl.hpp>
#include <SettingsFrame.hpp>

class MainFrame : public wxFrame {
    GitControl gCtrl;

    wxButton* initBttn;
    wxButton* pushBttn;
    wxButton* pullBttn;
    wxButton* settBttn;
    wxButton* trackNewBttn;

    SettingsFrame* settingsFrame;

    FileList* fileList;

    inline wxPoint getButtonOffset(wxButton* prev = nullptr) { return { prev ? prev->GetPosition().x + prev->GetSize().x + 10 : 10, 15 }; }

    void initEventBttn(wxCommandEvent&);
    void pushEventBttn(wxCommandEvent&);
    void pullEventBttn(wxCommandEvent&);
    void settEventBttn(wxCommandEvent&);

    void closeWinEvent(wxCloseEvent&);

    void standardExit(wxCloseEvent&, bool warnUnpushed = true);

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
