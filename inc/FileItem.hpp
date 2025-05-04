#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>

class FileItem : public wxPanel {
    wxTextCtrl* genNameField;
    wxTextCtrl* pathField;

public:
    static constexpr int itemWidth  { SS_GLOBALDEFS::WSX / 2 };
    static constexpr int itemHeight { 50 };
    const wxSize itemSize { itemWidth, itemHeight };

    static constexpr int itemMargin { 10 };

    std::string genericName;
    std::string filePath;

    FileItem(wxWindow* parent, std::string genericName, std::string filePath);
};
