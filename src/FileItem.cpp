#include <FileItem.hpp>

#include <ScatterSyncDefs.hpp>
#include <ManifestManip.hpp>
#include <UserFileControl.hpp>
#include <FileList.hpp>

#include <algorithm>

void fixSeperators(std::string& str) {
    if (str.length() == 0)
        return;

    std::replace(str.begin(), str.end(), '\\', '/');

    if (str[str.length() - 1] != '/')
        str.append({ '/' });
}

void FileItem::untrackEvent(wxCommandEvent& WXUNUSED(ce)) {
    static_cast<FileList*>(GetParent())->removeFileItem(elementIdent);
}

void FileItem::browserEvent(wxCommandEvent& WXUNUSED(ce)) {
    if (fileBrowser->ShowModal() == wxID_CANCEL)
        return;

    std::string directory { fileBrowser->GetDirectory() };
    std::string fileName  { fileBrowser->GetFilename() };

    fixSeperators(directory);

    dirField->SetValue(directory);
    nameField->SetValue(fileName);
}

FileItem::FileItem(wxWindow* parent, ManifestManip::Ident elementIdent)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { WINDOW_SIZE_X, itemHeight } }, elementIdent { elementIdent } {
    SetTransparent(true);

    genNameField = new wxTextCtrl { this, wxID_ANY, ManifestManip::genericNameOf(elementIdent), { 0, 0 }, itemSize };
    dirField     = new wxTextCtrl { this, wxID_ANY, ManifestManip::localDirOf(elementIdent), { itemWidth, 0 }, itemSize };
    nameField    = new wxTextCtrl { this, wxID_ANY, ManifestManip::fileNameOf(elementIdent), { 2 * itemWidth, 0 }, itemSize };

    fileBrowser = new wxFileDialog { this, "Open File", wxEmptyString, wxEmptyString, "Any Files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST };

    untrackBttn = new wxButton { this, wxID_ANY, "Untrack", { 3 * itemWidth, 0 } };
    untrackBttn->Bind(wxEVT_BUTTON, &FileItem::untrackEvent, this);

    fileBrowserBttn = new wxButton { this, wxID_ANY, "Browser", { 3 * itemWidth, untrackBttn->GetPosition().y + untrackBttn->GetClientSize().GetY() + 4 } };
    fileBrowserBttn->Bind(wxEVT_BUTTON, &FileItem::browserEvent, this);

    Show();
}

void FileItem::submitUpdate() {
    if (dirField->GetValue().length() > 0) {
        std::string directory { dirField->GetValue() };

        fixSeperators(directory);

        dirField->SetValue(directory);
    }

    ManifestManip::genericNameOf(elementIdent) = genNameField->GetValue();
    ManifestManip::localDirOf(elementIdent)    = dirField->GetValue();
    ManifestManip::fileNameOf(elementIdent)    = nameField->GetValue();
}
