#include <FileItem.hpp>

#include <ScatterSyncDefs.hpp>

#include <ManifestManip.hpp>
#include <UserFileControl.hpp>
#include <FileList.hpp>

void FileItem::removeEvent(wxCommandEvent& WXUNUSED(event)) {
    UserFileControl::takeAction(elementIdent, UserFileControl::Action::UNTRACK);
    dynamic_cast<FileList*>(GetParent())->intake();
}

FileItem::FileItem(wxWindow* parent, ManifestManip::Ident elementIdent)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { SS_GLOBALDEFS::WSX, itemHeight } }, elementIdent { elementIdent } {
    SetTransparent(true);

    genNameField = new wxTextCtrl { this, wxID_ANY, ManifestManip::genericNameOf(elementIdent), { 0, 0 }, itemSize };
    dirField     = new wxTextCtrl { this, wxID_ANY, ManifestManip::localDirOf(elementIdent), { itemWidth, 0 }, itemSize };
    nameField    = new wxTextCtrl { this, wxID_ANY, ManifestManip::fileNameOf(elementIdent), { 2 * itemWidth, 0 }, itemSize };

    removeBttn = new wxButton { this, wxID_ANY, "Untrack", { 3 * itemWidth, 0 }, { SS_GLOBALDEFS::WSX - 3 * itemWidth, -1 } };
    removeBttn->Bind(wxEVT_BUTTON, &FileItem::removeEvent, this);

    Show();

    
}

void FileItem::submitUpdate() {
    std::string directory { dirField->GetValue() };
    
    // Replace all occurences of '\' with '/'.
    bool popped { false };
    for (char& character : directory) {
        if (character == '\\') {
            character = '/';

            if (!popped) {
                popped = true;
                POPUP("Use '/' instead of '\\' in file directory field.")
            }
        }
    }

    if (directory.length() > 0 && directory[directory.length() - 1] != '/')
        dirField->SetValue(directory.append({ '/' }));

    ManifestManip::genericNameOf(elementIdent) = genNameField->GetValue();
    ManifestManip::localDirOf(elementIdent)    = dirField->GetValue();
    ManifestManip::fileNameOf(elementIdent)    = nameField->GetValue();
}
