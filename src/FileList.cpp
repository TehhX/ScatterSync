#include <FileList.hpp>

#include <ScatterSyncDefs.hpp>

constexpr int topMargin { 100 };
constexpr int scrollAmount { 10 };

FileList::FileList(wxWindow* parent)
: wxPanel { parent, wxID_ANY, { 0, topMargin }, { ssdef::WSX, ssdef::WSY - topMargin } } {
    SetBackgroundColour(ssdef::WHITE);
}

void FileList::addFileItem(FileItem* fileItem) {
    fileItem->SetParent(this);
    fileItem->SetPosition({ 0, static_cast<int>(itemHeight * fileItems.size() + itemMargin) });
    fileItems.push_back(fileItem);

    SetSize({ ssdef::WSX, static_cast<int>((itemHeight + 1) * fileItems.size() + itemMargin) });
}

void FileList::changeFileName(int index, std::string_view newName) {
    fileItems[index]->genericName = newName;
}

void FileList::changeFilePath(int index, std::string_view newPath) {
    fileItems[index]->filePath = newPath;
}

void FileList::scrollUp() {
    SetPosition({ 0, GetPosition().y + scrollAmount });
}

void FileList::scrollDown() {
    SetPosition({ 0, GetPosition().y - scrollAmount });
}
