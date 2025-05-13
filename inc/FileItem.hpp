#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>

class FileItem : public wxPanel {
    wxTextCtrl* genNameField;
    wxTextCtrl* pathField;
    // TODO: Should have a file browser here later for the pathField.

public:
    const u_llong uniqueIdent;

    static constexpr int itemWidth  { SS_GLOBALDEFS::WSX / 2 };
    static constexpr int itemHeight { 50 };
    const wxSize itemSize { itemWidth, itemHeight };

    static constexpr int itemMargin { 10 };

    FileItem(wxWindow* parent, u_llong uniqueIdent);

    void submitUpdate();
};
