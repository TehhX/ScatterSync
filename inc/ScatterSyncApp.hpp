#pragma once

#include <MainFrame.hpp>
#include <ManifestManip.hpp>
#include <UserFileControl.hpp>
#include <GitControl.hpp>

class ScatterSyncApp : public wxApp {
    ManifestManip manManip {};
    UserFileControl userCtrl {};
    GitControl gitCtrl {};

    MainFrame* mainFrame;

public:
    virtual bool OnInit() override final;
};
