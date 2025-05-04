#pragma once

#include <vector>
#include <string_view>

#include <wx/wx.h>

#include <FileItem.hpp>

class FileList : public wxPanel {
    std::vector<FileItem*> fileItems;
    int maxScroll { FileItem::itemMargin };

public:
    static constexpr int topMargin { 60 };
    static constexpr int scrollAmount { 10 };

    FileList(wxWindow* parent);

    void addFileItem(FileItem* fileItem);
    void addFileItem(std::string name, std::string path);

    void scrollUp();
    void scrollDown();
};
