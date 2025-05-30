#pragma once

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>
#include <FileItem.hpp>

#include <map>
#include <string>

#include <wx/panel.h>

class FileList : public wxPanel {
    std::map<ManifestManip::Ident, FileItem*> fileItems;

    static constexpr int maxScrollDefault { FileItem::itemMargin - WINDOW_SIZE_Y };
    int maxScroll { maxScrollDefault };

    void scroll(wxMouseEvent& me);

    void scrollBoundsCheck();

public:
    static constexpr int topMargin { 60 };

    FileList(wxWindow* parent);

    void intake();

    void createNewFile(wxCommandEvent&);

    void removeFileItem(ManifestManip::Ident ident);

    void addFileItem(ManifestManip::Ident ident);

    void submitAllUpdates();
};
