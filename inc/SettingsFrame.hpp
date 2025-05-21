#pragma once

#include <ScatterSyncDefs.hpp>

#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>

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
