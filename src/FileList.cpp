#include <FileList.hpp>

#include <ScatterSyncDefs.hpp>

#include <MainFrame.hpp>
#include <ManifestManip.hpp>

#include <functional>

void FileList::createNewFile(wxCommandEvent& WXUNUSED(event)) {
    addFileItem(ManifestManip::createNewFileMap()->first);
}

FileList::FileList(wxWindow* parent)
: wxPanel { parent, wxID_ANY, { 0, topMargin }, { SS_GLOBALDEFS::WSX, SS_GLOBALDEFS::WSY - topMargin } } {
    Hide();
    SetBackgroundColour(SS_GLOBALDEFS::DARK_GREY);
    parent->Bind(wxEVT_MOUSEWHEEL, &FileList::scroll, this);

    ManifestManip::forEach({ [this](const ManifestManip::UFIMap::iterator& map) -> void {
        addFileItem(map->first);
    }});

    Show();
}

void FileList::addFileItem(u_llong uniqueIdent) {
    auto fileItem = new FileItem { this, uniqueIdent };
    fileItem->SetBackgroundColour(SS_GLOBALDEFS::DARK_GREY);
    fileItem->SetPosition({ 0, SC(int, FileItem::itemHeight * fileItems.size() + FileItem::itemMargin) });
    fileItem->Show();

    SetSize({ SS_GLOBALDEFS::WSX, SC(int, FileItem::itemHeight * (fileItems.size() + 1) + FileItem::itemMargin) });

    fileItems.push_back(fileItem);
    maxScroll += FileItem::itemHeight;
}

void FileList::scroll(wxMouseEvent& me) {
    if (me.GetWheelRotation() > 0) {
        SetPosition({ 0, GetPosition().y + MainFrame::settings.scrollSpeed });

        if (GetPosition().y > topMargin)
            SetPosition({ 0, topMargin });
    } else {
        SetPosition({ 0, GetPosition().y - MainFrame::settings.scrollSpeed });

        if (GetPosition().y < -maxScroll)
            SetPosition({ 0, -maxScroll });
    }
}

void FileList::submitAllUpdates() {
    for (auto item : fileItems)
        item->submitUpdate();
}
