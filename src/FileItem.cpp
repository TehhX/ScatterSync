#include <FileItem.hpp>

#include <ScatterSyncDefs.hpp>

#include <ManifestManip.hpp>

FileItem::FileItem(wxWindow* parent, ManifestManip::Ident elementIdent)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { SS_GLOBALDEFS::WSX, itemHeight } }, elementIdent { elementIdent } {
    SetBackgroundColour(SS_GLOBALDEFS::LIGHT_GREY);

    genNameField = new wxTextCtrl { this, wxID_ANY, ManifestManip::genericNameOf(elementIdent), { 0, 0 }, itemSize };
    pathField = new wxTextCtrl    { this, wxID_ANY, ManifestManip::localPathOf(elementIdent), { itemWidth, 0 }, itemSize };
}

void FileItem::submitUpdate() {
    ManifestManip::genericNameOf(elementIdent) = genNameField->GetValue();
    ManifestManip::localPathOf(elementIdent)   = pathField->GetValue();
}
