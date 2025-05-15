#pragma once

#include <ScatterSyncDefs.hpp>

class SettingsFrame : public wxFrame {
    static inline const wxSize buttonSize { 140, -1 };

    wxStaticText* scrollSpeedLabel;

    wxTextCtrl* scrollSpeed;

    wxCheckBox* autoSync;
    wxCheckBox* promptUnpushed;
    wxCheckBox* initGitOnOpen;

    void onCloseEvent(wxCloseEvent&);

public:
    SettingsFrame(wxWindow* parent);
};
