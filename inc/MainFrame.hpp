#pragma once

#include <FileList.hpp>
#include <GitControl.hpp>

class MainFrame : public wxFrame {
    GitControl gCtrl;

    wxButton* initBttn;
    wxButton* pushBttn;
    wxButton* pullBttn;

    FileList* fileList;

    void initEventBttn(wxCommandEvent& WXUNUSED(event));
    void pushEventBttn(wxCommandEvent& WXUNUSED(event));
    void pullEventBttn(wxCommandEvent& WXUNUSED(event));

    void closeWinEvent(wxCloseEvent&   WXUNUSED(event));

public:
    struct Settings {
        long autoSyncSeconds { -1 };
        bool autoSyncOnOpen { false };
        short scrollSpeed { 7 };
        bool exitPromptUnpushed { true };
    };

    static inline Settings settings {};

    MainFrame();
};
