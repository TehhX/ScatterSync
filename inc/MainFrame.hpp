#pragma once

#include <FileList.hpp>

class MainFrame : public wxFrame {
    wxButton* initBttn;
    wxButton* pushBttn;
    wxButton* pullBttn;

    FileList* fileList;

    void initEventBttn(wxCommandEvent& WXUNUSED(event));
    void pushEventBttn(wxCommandEvent& WXUNUSED(event));
    void pullEventBttn(wxCommandEvent& WXUNUSED(event));

public:
    MainFrame();
};
