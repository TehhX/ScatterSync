#pragma once

#include <vector>
#include <string>

#include <FileItem.hpp>

class FileList : public wxPanel {
    std::vector<FileItem*> fileItems;
    int maxScroll { FileItem::itemMargin };

public:
    static constexpr int topMargin { 60 };
    static constexpr int scrollAmount { 10 };

    FileList(wxWindow* parent);

    void addFileItem(const std::string& name, const std::string& path);

    void scrollUp();
    void scrollDown();
};
