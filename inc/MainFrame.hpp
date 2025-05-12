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

    SettingsFrame* settingsFrame;

    FileList* fileList;

    void initEventBttn(wxCommandEvent&);
    void pushEventBttn(wxCommandEvent&);
    void pullEventBttn(wxCommandEvent&);
    void settEventBttn(wxCommandEvent&);

    void closeWinEvent(wxCloseEvent&);

public:
    struct Settings {
        long autoSyncSeconds;
        bool autoSyncOnOpen;
        int scrollSpeed;
        bool exitPromptUnpushed;
    };

    static inline Settings settings { 0, false, 7, true };

    MainFrame();
};
