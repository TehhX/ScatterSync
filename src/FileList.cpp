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

    scrollInBounds();
}

void FileList::addFileItem(ManifestManip::Ident ident) {
    fileItems.insert({ ident, new FileItem { this, ident } });
    fileItems.find(ident)->second->SetPosition({ 0, SC(int, (FileItem::itemHeight + FileItem::itemMargin) * (fileItems.size() - 1)) });

    maxScroll += FileItem::itemHeight + FileItem::itemMargin;
    resetSize();

    scrollInBounds();
}

void FileList::scroll(wxMouseEvent& me) {
    int rot { me.GetWheelRotation() };
    scrollInBounds(SIGN(rot, SC_UP, SC_DOWN, SC_NONE));
}

void FileList::scrollInBounds(Direction d) {
    if ((FileItem::itemMargin + FileItem::itemHeight) * fileItems.size() - FileItem::itemMargin <= WINDOW_SIZE_Y) {
        SetPosition({ 0, 0 });
        return;
    }

    switch (d) {
    default:
        break;

    case SC_UP:
        SetPosition({ 0, SC(int, std::min(0, GetPosition().y + SC(int, MainFrame::settings.scrollSpeed))) });
        break;

    case SC_DOWN:
        SetPosition({ 0, SC(int, std::max(-maxScroll, GetPosition().y - SC(int, MainFrame::settings.scrollSpeed))) });
        break;
    }
}

void FileList::submitAllUpdates() {
    for (auto mapPair : fileItems)
        mapPair.second->submitUpdate();
}
