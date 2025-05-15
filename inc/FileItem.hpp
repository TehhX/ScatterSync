#pragma once

#include <ScatterSyncDefs.hpp>

#include <ManifestManip.hpp>

#include <string>

class FileItem : public wxPanel {
    wxTextCtrl* genNameField;
    wxTextCtrl* dirField;
    wxTextCtrl* nameField;

    // TODO: Should have a file browser here later for the pathField.
    // TODO: Should have a delete button here later (maybe)

public:
    const ManifestManip::Ident elementIdent;

    static constexpr int itemWidth  { SS_GLOBALDEFS::WSX / 3 };
    static constexpr int itemHeight { 50 };
    const wxSize itemSize { itemWidth, itemHeight };

    static constexpr int itemMargin { 10 };

    FileItem(wxWindow* parent, ManifestManip::Ident elementIdent);

    void submitUpdate();
};
