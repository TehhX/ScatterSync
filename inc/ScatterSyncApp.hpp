#pragma once

#include <wx/app.h>
#include <wx/frame.h>

#include <ManifestManip.hpp>
#include <UserFileControl.hpp>
#include <GitControl.hpp>

class ScatterSyncApp : public wxApp {
    ManifestManip manManip {};
    UserFileControl userCtrl {};
    GitControl gitCtrl {};

    wxFrame* mainFrame;

    virtual bool OnInit() override final;
};
