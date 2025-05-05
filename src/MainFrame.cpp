#include <MainFrame.hpp>

#include <ScatterSyncDefs.hpp>

MainFrame::MainFrame()
: wxFrame { nullptr, wxID_ANY, "Scatter Sync" } {
    SetClientSize(SS_GLOBALDEFS::WSX, SS_GLOBALDEFS::WSY);
    SetBackgroundColour(SS_GLOBALDEFS::DARK_GREY);
    Center();
    Show();

    fileList = new FileList { this };

    initBttn = new wxButton { this, wxID_ANY, "Init", { 30, 10 }, { 60, -1 } };
    pushBttn = new wxButton { this, wxID_ANY, "Push", { 100, 10 }, { 60, -1 } };
    pullBttn = new wxButton { this, wxID_ANY, "Pull", { 170, 10 }, { 60, -1 } };

    Show();
}
