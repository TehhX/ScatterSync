#pragma once

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>
#include <FileItem.hpp>

#include <map>
#include <string>

#include <wx/panel.h>

class FileList : public wxPanel {
    std::map<ManifestManip::Ident, FileItem*> fileItems;

    int maxScroll { FileItem::itemMargin - WINDOW_SIZE_Y };

    void scroll(wxMouseEvent& me);

public:
    static constexpr int topMargin { 60 };

    FileList(wxWindow* parent);

    void intake();

    void createNewFile(wxCommandEvent&);

    void addFileItem(ManifestManip::Ident ident);

    void submitAllUpdates();
};
