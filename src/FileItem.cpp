#include <FileItem.hpp>

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>
#include <UserFileControl.hpp>
#include <FileList.hpp>

#include <algorithm>

void FileItem::removeEvent(wxCommandEvent& WXUNUSED(event)) {
    UserFileControl::takeAction(elementIdent, UserFileControl::Action::UNTRACK);
    dynamic_cast<FileList*>(GetParent())->intake();
}

FileItem::FileItem(wxWindow* parent, ManifestManip::Ident elementIdent)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { WINDOW_SIZE_X, itemHeight } }, elementIdent { elementIdent } {
    SetTransparent(true);

    genNameField = new wxTextCtrl { this, wxID_ANY, ManifestManip::genericNameOf(elementIdent), { 0, 0 }, itemSize };
    dirField     = new wxTextCtrl { this, wxID_ANY, ManifestManip::localDirOf(elementIdent), { itemWidth, 0 }, itemSize };
    nameField    = new wxTextCtrl { this, wxID_ANY, ManifestManip::fileNameOf(elementIdent), { 2 * itemWidth, 0 }, itemSize };

    removeBttn = new wxButton { this, wxID_ANY, "Untrack", { 3 * itemWidth, 0 }, { WINDOW_SIZE_X - 3 * itemWidth, -1 } };
    removeBttn->Bind(wxEVT_BUTTON, &FileItem::removeEvent, this);

    Show();
}

void FileItem::submitUpdate() {
    if (dirField->GetValue().length() > 0) {
        std::string directory { dirField->GetValue() };

        std::replace(directory.begin(), directory.end(), '\\', '/');

        if (directory[directory.length() - 1] != '/')
            directory.append({ '/' });

        dirField->SetValue(directory);
    }

    ManifestManip::genericNameOf(elementIdent) = genNameField->GetValue();
    ManifestManip::localDirOf(elementIdent)    = dirField->GetValue();
    ManifestManip::fileNameOf(elementIdent)    = nameField->GetValue();
}
