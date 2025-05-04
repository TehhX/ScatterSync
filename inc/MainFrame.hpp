#pragma once

#include <wx/wx.h>

#include <FileList.hpp>

class MainFrame : public wxFrame {
    wxButton* pushBttn;
    wxButton* pullBttn;
    FileList* fileList;

public:
    MainFrame();
};
