#pragma once

#include <ScatterSyncDefs.hpp>

#include <MainFrame.hpp>

class ScatterSyncApp final : public wxApp {
    MainFrame* mainFrame;

public:
    virtual bool OnInit() wxOVERRIDE;
};
