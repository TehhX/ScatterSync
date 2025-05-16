#pragma once

#include <ScatterSyncDefs.hpp>

#include <ManifestManip.hpp>

#include <string>

class FileItem : public wxPanel {
    wxWindow* const parent;
    wxTextCtrl* genNameField;
    wxTextCtrl* dirField;
    wxTextCtrl* nameField;

    wxButton*   removeBttn;

    void removeEvent(wxCommandEvent& ce);

    // TODO: Should have a file browser here later for the pathField.

public:
    const ManifestManip::Ident elementIdent;

    static constexpr int removeWidth { 100 };
    static constexpr int itemWidth  { SS_GLOBALDEFS::WSX / 3 - removeWidth / 3 };
    static constexpr int itemHeight { 50 };
    const wxSize itemSize { itemWidth, itemHeight };

    static constexpr int itemMargin { 10 };

    FileItem(wxWindow* parent, ManifestManip::Ident elementIdent);

    void submitUpdate();
};
