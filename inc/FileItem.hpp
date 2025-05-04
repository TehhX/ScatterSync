#pragma once

#include <string>

#include <wx/wx.h>

constexpr int itemHeight { 50 };
constexpr int itemMargin { 10 };

class FileItem : public wxPanel {
public:
    std::string genericName;
    std::string filePath;

    FileItem(wxWindow* parent, std::string_view genericName, std::string_view filePath);
};
