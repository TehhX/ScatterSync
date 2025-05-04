#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>

#include <wx/wx.h>

class FileItem : public wxPanel {
    wxTextCtrl* genNameField;
    wxTextCtrl* pathField;

public:
    static constexpr int itemWidth  { ssdef::WSX };
    static constexpr int itemHeight { 50 };
    const wxSize itemSize { itemWidth, itemHeight };

    static constexpr int itemMargin { 10 };
    static constexpr int halfInset  { (ssdef::WSX - itemWidth) / 2 };

    std::string genericName;
    std::string filePath;

    FileItem(wxWindow* parent, std::string genericName, std::string filePath);
};
