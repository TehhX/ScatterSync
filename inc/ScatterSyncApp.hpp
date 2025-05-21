#pragma once

#include <ScatterSyncDefs.hpp>
#include <MainFrame.hpp>

#include <wx/app.h>

class ScatterSyncApp final : public wxApp {
    MainFrame* mainFrame;

public:
    virtual bool OnInit() wxOVERRIDE;
};
