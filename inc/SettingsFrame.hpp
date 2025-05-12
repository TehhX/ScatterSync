#pragma once

#include <ScatterSyncDefs.hpp>

class SettingsFrame : public wxFrame {
    static inline const wxSize buttonSize { 140, -1 };

    wxStaticText* autoSyncLabel;
    wxStaticText* scrollSpeedLabel;

    wxTextCtrl* autoSyncDelta;
    wxTextCtrl* scrollSpeed;

    wxCheckBox* autoSync;
    wxCheckBox* promptUnpushed;

    void onCloseEvent(wxCloseEvent&);

public:
    SettingsFrame(wxWindow* parent);
};
