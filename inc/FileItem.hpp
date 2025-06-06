#pragma once

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>

#include <string>

#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/filedlg.h>

class FileItem : public wxPanel {
    wxTextCtrl* genNameField;
    wxTextCtrl* dirField;
    wxTextCtrl* nameField;

    wxFileDialog* fileBrowser;

    wxButton* untrackBttn;
    wxButton* fileBrowserBttn;

    void untrackEvent(wxCommandEvent& ce);

    void browserEvent(wxCommandEvent& ce);

public:
    const ManifestManip::Ident elementIdent;

    static constexpr int removeWidth { 100 };
    static constexpr int itemHeight  {  50 };
    static constexpr int itemMargin  {  10 };
    static constexpr int itemWidth   { WINDOW_SIZE_X / 3 - removeWidth / 3 };

    const wxSize itemSize { itemWidth, itemHeight };

    FileItem(wxWindow* parent, ManifestManip::Ident elementIdent);

    void submitUpdate();
};
