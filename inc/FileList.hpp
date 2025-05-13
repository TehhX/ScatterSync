#pragma once

#include <vector>
#include <string>

#include <ScatterSyncDefs.hpp>

#include <FileItem.hpp>

class FileList : public wxPanel {
    std::vector<FileItem*> fileItems;

    int maxScroll { FileItem::itemMargin - SS_GLOBALDEFS::WSY };

    void scroll(wxMouseEvent& me);

public:
    static constexpr int topMargin { 60 };

    FileList(wxWindow* parent);

    void createNewFile(wxCommandEvent&);

    void addFileItem(size_t index);

    void submitAllUpdates();
};
