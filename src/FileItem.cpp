#include <FileItem.hpp>

#include <ScatterSyncDefs.hpp>

FileItem::FileItem(wxWindow* parent, std::string_view genericName, std::string_view filePath)
: wxPanel { parent, wxID_ANY, wxDefaultPosition, { ssdef::WSX, 0 } } {
    
}
