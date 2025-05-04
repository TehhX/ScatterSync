#include <MainFrame.hpp>

#include <ScatterSyncDefs.hpp>

MainFrame::MainFrame()
: wxFrame { nullptr, wxID_ANY, "Scatter Sync" } {
    SetClientSize(ssdef::WSX, ssdef::WSY);
    SetBackgroundColour(ssdef::DARK_GREY);
    Center();

    pushBttn = new wxButton { this, wxID_ANY, "Push", { 30, 20 }, { 60, -1 }};
    pullBttn = new wxButton { this, wxID_ANY, "Pull", { 100, 20 }, { 60, -1 }};
    fileList = new FileList { this };

    Show();
}
