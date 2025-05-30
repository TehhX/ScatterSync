#include <FileList.hpp>

#include <ScatterSyncDefs.hpp>
#include <MainFrame.hpp>
#include <ManifestManip.hpp>

#define resetSize() SetClientSize({ WINDOW_SIZE_X, SC(int, (FileItem::itemHeight + FileItem::itemMargin) * fileItems.size()) - maxScrollDefault })

FileList::FileList(wxWindow* parent)
: wxPanel { parent, wxID_ANY } {
    parent->Bind(wxEVT_MOUSEWHEEL, &FileList::scroll, this);

    intake();
}

void FileList::intake() {
    for (auto& pair : fileItems)
        pair.second->Destroy();

    maxScroll = maxScrollDefault;
    fileItems.clear();

    maniManiForEach(addFileItem(ident);)
}

void FileList::createNewFile(wxCommandEvent& WXUNUSED(event)) {
    ManifestManip::Ident ident { ManifestManip::createNewFileElement() };

    addFileItem(ident);
}

void FileList::removeFileItem(ManifestManip::Ident ident) {
    UserFileControl::takeAction(ident, UserFileControl::Action::UNTRACK);
    intake();

    resetSize();

    scrollBoundsCheck();
}

void FileList::addFileItem(ManifestManip::Ident ident) {
    fileItems.insert({ ident, new FileItem { this, ident } });
    fileItems.find(ident)->second->SetPosition({ 0, SC(int, (FileItem::itemHeight + FileItem::itemMargin) * (fileItems.size() - 1)) });

    maxScroll += FileItem::itemHeight + FileItem::itemMargin;
    resetSize();

    scrollBoundsCheck();
}

void FileList::scroll(wxMouseEvent& me) {
    if (me.GetWheelRotation() > 0)
        SetPosition({ 0, GetPosition().y + SC(int, MainFrame::settings.scrollSpeed) });
    else
        SetPosition({ 0, GetPosition().y - SC(int, MainFrame::settings.scrollSpeed) });

    scrollBoundsCheck();
}

void FileList::scrollBoundsCheck() {
    // Are enough elements to check bounds?
    if ((FileItem::itemMargin + FileItem::itemHeight) * fileItems.size() - FileItem::itemMargin <= WINDOW_SIZE_Y) {
        SetPosition({ 0, 0 });
        return;
    }

    if (GetPosition().y > 0)
        SetPosition({ 0, 0 });

    else if (GetPosition().y < -maxScroll)
        SetPosition({ 0, -maxScroll });
}

void FileList::submitAllUpdates() {
    for (auto mapPair : fileItems)
        mapPair.second->submitUpdate();
}
