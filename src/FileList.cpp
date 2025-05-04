#include <FileList.hpp>

#include <ScatterSyncDefs.hpp>

FileList::FileList(wxWindow* parent)
: wxPanel { parent, wxID_ANY, { 0, topMargin }, { SS_GLOBALDEFS::WSX, SS_GLOBALDEFS::WSY - topMargin } } {
    SetBackgroundColour(SS_GLOBALDEFS::LIGHT_GREY);
    Show();

    for (size_t i { 0 }; i < 50; i++) {
        addFileItem(std::to_string(i), std::to_string(i));
    }
}

void FileList::addFileItem(const std::string& name, const std::string& path) {
    auto fileItem = new FileItem { this, name, path };
    fileItem->SetPosition({ 0, static_cast<int>(FileItem::itemHeight * fileItems.size() + FileItem::itemMargin) });
    fileItem->Show();

    SetSize({ SS_GLOBALDEFS::WSX, static_cast<int>(FileItem::itemHeight * (fileItems.size() + 1) + FileItem::itemMargin) });
    
    fileItems.push_back(fileItem);
    maxScroll += FileItem::itemHeight;
}

void FileList::scrollUp() {
    SetPosition({ 0, GetPosition().y + scrollAmount });

    if (GetPosition().y < 0)
        SetPosition({ 0, 0 });
}

void FileList::scrollDown() {
    SetPosition({ 0, GetPosition().y - scrollAmount });

    if (GetPosition().y > maxScroll)
        SetPosition({ 0, maxScroll });
}
