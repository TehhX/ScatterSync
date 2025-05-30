#include <FileList.hpp>

#include <ScatterSyncDefs.hpp>
#include <MainFrame.hpp>
#include <ManifestManip.hpp>

#define resetSize() SetClientSize({ WINDOW_SIZE_X, SC(int, FileItem::itemHeight * fileItems.size() + FileItem::itemMargin) })

FileList::FileList(wxWindow* parent)
: wxPanel { parent, wxID_ANY, { 0, topMargin }, { WINDOW_SIZE_X, WINDOW_SIZE_Y - topMargin } } {
    SetTransparent(true);
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
    fileItems.find(ident)->second->SetPosition({ 0, SC(int, FileItem::itemHeight * (fileItems.size() - 1) + FileItem::itemMargin) });

    maxScroll += FileItem::itemHeight;
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
    if (FileItem::itemMargin + FileItem::itemHeight * fileItems.size() <= WINDOW_SIZE_Y - topMargin)
        return SetPosition({ 0, topMargin });

    if (GetPosition().y > topMargin)
        SetPosition({ 0, topMargin });

    else if (GetPosition().y < -maxScroll)
        SetPosition({ 0, -maxScroll });
}

void FileList::submitAllUpdates() {
    for (auto mapPair : fileItems)
        mapPair.second->submitUpdate();
}
