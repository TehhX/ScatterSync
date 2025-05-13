#include <FileItem.hpp>

#include <ScatterSyncDefs.hpp>

#include <ManifestManip.hpp>

FileItem::FileItem(wxWindow* parent, size_t elementIndex)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { SS_GLOBALDEFS::WSX, itemHeight } }, elementIndex { elementIndex } {
    SetBackgroundColour(SS_GLOBALDEFS::LIGHT_GREY);

    genNameField = new wxTextCtrl { this, wxID_ANY, ManifestManip::genericNameOf(elementIndex), { 0, 0 }, itemSize };
    pathField = new wxTextCtrl    { this, wxID_ANY, ManifestManip::localPathOf(elementIndex), { itemWidth, 0 }, itemSize };
}

void FileItem::submitUpdate() {
    ManifestManip::genericNameOf(elementIndex) = genNameField->GetValue();
    ManifestManip::localPathOf(elementIndex)   = pathField->GetValue();
}
