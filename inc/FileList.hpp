#pragma once

#include <vector>

#include <wx/wx.h>

#include <FileItem.hpp>

class FileList : public wxPanel {
    std::vector<FileItem*> fileItems;

public:
    FileList(wxWindow* parent);

    void addFileItem(FileItem* fileItem);

    void changeFileName(int index, std::string_view newName);
    void changeFilePath(int index, std::string_view newPath);

    void scrollUp();
    void scrollDown();
};
