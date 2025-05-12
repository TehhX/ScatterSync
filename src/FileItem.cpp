#include <FileItem.hpp>

#include <ScatterSyncDefs.hpp>

#include <ManifestManip.hpp>

FileItem::FileItem(wxWindow* parent, u_llong uniqueIdent)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { SS_GLOBALDEFS::WSX, itemHeight } }, uniqueIdent { uniqueIdent } {
    SetBackgroundColour(SS_GLOBALDEFS::LIGHT_GREY);

    genNameField = new wxTextCtrl { this, wxID_ANY, ManifestManip::genNameOf(uniqueIdent), { 0, 0 }, itemSize };
    pathField = new wxTextCtrl { this, wxID_ANY, ManifestManip::localPathOf(uniqueIdent), { itemWidth, 0 }, itemSize };
}

void FileItem::submitUpdate() {
    ManifestManip::genNameOf(uniqueIdent)   = genNameField->GetValue();
    ManifestManip::localPathOf(uniqueIdent) = pathField->GetValue();
}
