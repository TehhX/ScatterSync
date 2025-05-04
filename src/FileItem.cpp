#include <FileItem.hpp>

#include <ScatterSyncDefs.hpp>

FileItem::FileItem(wxWindow* parent, std::string genericName, std::string filePath)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { SS_GLOBALDEFS::WSX, itemHeight } } {
    SetBackgroundColour(SS_GLOBALDEFS::LIGHT_GREY);
    genNameField = new wxTextCtrl { this, wxID_ANY, genericName, { 0, 0 }, itemSize };
    pathField = new wxTextCtrl { this, wxID_ANY, filePath, { itemWidth, 0 }, itemSize };
}
