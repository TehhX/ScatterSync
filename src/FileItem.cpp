#include <FileItem.hpp>

FileItem::FileItem(wxWindow* parent, std::string genericName, std::string filePath)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { ssdef::WSX, 0 } } {
    genNameField = new wxTextCtrl { this, wxID_ANY, genericName, { 0, 0 }, itemSize };
    pathField = new wxTextCtrl { this, wxID_ANY, filePath, { halfInset, 0 }, itemSize };
}
