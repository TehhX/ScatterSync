#pragma once

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>
#include <FileItem.hpp>

#include <map>

#include <wx/panel.h>

class FileList : public wxPanel {
    enum Direction {
        SC_UP,
        SC_DOWN,
        SC_NONE
    };

    std::map<ManifestManip::Ident, FileItem*> fileItems;

    static constexpr int maxScrollDefault { FileItem::itemMargin - WINDOW_SIZE_Y };
    int maxScroll { maxScrollDefault };

    void scroll(wxMouseEvent& me);

    void scrollInBounds(Direction d = SC_NONE);

public:
    FileList(wxWindow* parent);

    void intake();

    void createNewFile(wxCommandEvent&);

    void removeFileItem(ManifestManip::Ident ident);

    void addFileItem(ManifestManip::Ident ident);

    void submitAllUpdates();
};
