#pragma once

#include <FileList.hpp>

class MainFrame : public wxFrame {
    wxButton* initBttn;
    wxButton* pushBttn;
    wxButton* pullBttn;

    FileList* fileList;

public:
    MainFrame();
};
