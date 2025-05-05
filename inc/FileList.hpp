#pragma once

#include <vector>
#include <string>

#include <ScatterSyncDefs.hpp>

#include <FileItem.hpp>

class FileList : public wxPanel {
    std::vector<FileItem*> fileItems;
    int maxScroll { FileItem::itemMargin - SS_GLOBALDEFS::WSY };

public:
    static constexpr int topMargin { 60 };
    static constexpr int scrollAmount { 7 };

    FileList(wxWindow* parent);

    void addFileItem(const std::string& name, const std::string& path);

    void scroll(wxMouseEvent& me);
};
