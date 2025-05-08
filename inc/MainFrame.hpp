#pragma once

#include <FileList.hpp>
#include <GitControl.hpp>

class MainFrame : public wxFrame {
    GitControl gCtrl;

    wxButton* initBttn;
    wxButton* pushBttn;
    wxButton* pullBttn;
    wxButton* settBttn;

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
        short scrollSpeed;
        bool exitPromptUnpushed;
    };

    static inline Settings settings { -1, false, 7, true };

    MainFrame();
};
