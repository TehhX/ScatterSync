#include <FileList.hpp>

#include <ScatterSyncDefs.hpp>

FileList::FileList(wxWindow* parent)
: wxPanel { parent, wxID_ANY, { 0, topMargin }, { ssdef::WSX, ssdef::WSY - topMargin } } {
    SetBackgroundColour(ssdef::WHITE);

    for (size_t i { 0 }; i < 50; i++) {
        addFileItem(std::to_string(i), std::to_string(i));
    }
}

void FileList::addFileItem(FileItem* fileItem) {
    fileItem->SetBackgroundColour({ 255, 0, 0 });
    fileItem->SetParent(this);
    fileItem->SetPosition({ 0, static_cast<int>(FileItem::itemHeight * fileItems.size() + FileItem::itemMargin) });
    fileItems.push_back(fileItem);

    SetSize({ ssdef::WSX, static_cast<int>((FileItem::itemHeight + 1) * fileItems.size() + FileItem::itemMargin) });
    maxScroll += FileItem::itemHeight;
}

void FileList::addFileItem(std::string name, std::string path) {
    auto newFileItem = new FileItem {this, name, path};

    addFileItem(newFileItem);
}

void FileList::scrollUp() {
    SetPosition({ 0, GetPosition().y - scrollAmount });

    if (GetPosition().y < 0)
        SetPosition({ 0, 0 });
}

void FileList::scrollDown() {
    SetPosition({ 0, GetPosition().y + scrollAmount });

    if (GetPosition().y > maxScroll)
        SetPosition({ 0, maxScroll });
}
