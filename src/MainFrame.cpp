#include <MainFrame.hpp>

#include <ScatterSyncDefs.hpp>

MainFrame::MainFrame()
: wxFrame { nullptr, wxID_ANY, "Scatter Sync" } {
    SetClientSize(SS_GLOBALDEFS::WSX, SS_GLOBALDEFS::WSY);
    SetBackgroundColour(SS_GLOBALDEFS::DARK_GREY);
    Center();

    fileList = new FileList { this };

    initBttn = new wxButton { this, wxID_ANY, "Init", { 30, 10 }, { 60, -1 } };
    initBttn->Bind(wxEVT_BUTTON, &MainFrame::initEventBttn, this);

    pushBttn = new wxButton { this, wxID_ANY, "Push", { 100, 10 }, { 60, -1 } };
    pushBttn->Bind(wxEVT_BUTTON, &MainFrame::pushEventBttn, this);

    pullBttn = new wxButton { this, wxID_ANY, "Pull", { 170, 10 }, { 60, -1 } };
    pullBttn->Bind(wxEVT_BUTTON, &MainFrame::pullEventBttn, this);

    Show();
}

void MainFrame::initEventBttn(wxCommandEvent& WXUNUSED(event)) {
    // TODO
}

void MainFrame::pushEventBttn(wxCommandEvent& WXUNUSED(event)) {
    // TODO
}

void MainFrame::pullEventBttn(wxCommandEvent& WXUNUSED(event)) {
    // TODO
}
