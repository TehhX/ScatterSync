#pragma once

#include <MainFrame.hpp>
#include <ManifestManip.hpp>
#include <UserFileControl.hpp>
#include <GitControl.hpp>

class ScatterSyncApp final : public wxApp {
    MainFrame* mainFrame;

public:
    virtual bool OnInit() override final;
};
