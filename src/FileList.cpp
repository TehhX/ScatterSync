#include <FileList.hpp>

#include <ScatterSyncDefs.hpp>

FileList::FileList(wxWindow* parent)
: wxScrolledWindow { parent, wxID_ANY, { 0, 10 }, { ssdef::WSX, ssdef::WSY - 10 } } {
    
}
