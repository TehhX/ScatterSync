#include <FileList.hpp>

#include <ScatterSyncDefs.hpp>

#include <MainFrame.hpp>
#include <ManifestManip.hpp>

void FileList::createNewFile(wxCommandEvent& WXUNUSED(event)) {
    ManifestManip::Ident ident { ManifestManip::createNewFileElement() };

    addFileItem(ident);
}

FileList::FileList(wxWindow* parent)
: wxPanel { parent, wxID_ANY, { 0, topMargin }, { SS_GLOBALDEFS::WSX, SS_GLOBALDEFS::WSY - topMargin } } {
    Hide();
    SetBackgroundColour(SS_GLOBALDEFS::DARK_GREY);
    parent->Bind(wxEVT_MOUSEWHEEL, &FileList::scroll, this);

    intake();

    Show();
}

void FileList::intake() {
    for (auto& pair : fileItems)
        pair.second->Destroy();

    fileItems.clear();
    MANI_FOR_EACH(addFileItem(ident);)
}

void FileList::addFileItem(ManifestManip::Ident ident) {
    fileItems.insert({ ident, new FileItem { this, ident } });
    fileItems.find(ident)->second->SetPosition({ 0, SC(int, FileItem::itemHeight * (fileItems.size() - 1) + FileItem::itemMargin) });

    maxScroll += FileItem::itemHeight;
    SetSize({ SS_GLOBALDEFS::WSX, SC(int, FileItem::itemHeight * fileItems.size() + FileItem::itemMargin) });
}

void FileList::scroll(wxMouseEvent& me) {
    if (me.GetWheelRotation() > 0) {
        SetPosition({ 0, GetPosition().y + SC(int, MainFrame::settings.scrollSpeed) });

        if (GetPosition().y > topMargin)
            SetPosition({ 0, topMargin });
    } else {
        SetPosition({ 0, GetPosition().y - SC(int, MainFrame::settings.scrollSpeed) });

        if (GetPosition().y < -maxScroll)
            SetPosition({ 0, -maxScroll });
    }
}

void FileList::submitAllUpdates() {
    for (auto mapPair : fileItems)
        mapPair.second->submitUpdate();
}
