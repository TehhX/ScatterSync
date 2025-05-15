#include <FileItem.hpp>

#include <ScatterSyncDefs.hpp>

#include <ManifestManip.hpp>

FileItem::FileItem(wxWindow* parent, ManifestManip::Ident elementIdent)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { SS_GLOBALDEFS::WSX, itemHeight } }, elementIdent { elementIdent } {
    SetBackgroundColour(SS_GLOBALDEFS::LIGHT_GREY);

    genNameField = new wxTextCtrl { this, wxID_ANY, ManifestManip::genericNameOf(elementIdent), { 0, 0 }, itemSize };
    dirField     = new wxTextCtrl { this, wxID_ANY, ManifestManip::localDirOf(elementIdent), { itemWidth, 0 }, itemSize };
    nameField    = new wxTextCtrl { this, wxID_ANY, ManifestManip::fileNameOf(elementIdent), { 2 * itemWidth, 0 }, itemSize };
}

void FileItem::submitUpdate() {
    std::string directory { dirField->GetValue() };
    if (directory.length() > 0 && directory[directory.length() - 1] != '/')
        dirField->SetValue(directory.append({ '/' }));

    ManifestManip::genericNameOf(elementIdent) = genNameField->GetValue();
    ManifestManip::localDirOf(elementIdent)    = dirField->GetValue();
    ManifestManip::fileNameOf(elementIdent)    = nameField->GetValue();
}
