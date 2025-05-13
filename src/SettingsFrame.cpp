#include <SettingsFrame.hpp>

#include <MainFrame.hpp>

void SettingsFrame::onCloseEvent(wxCloseEvent& WXUNUSED(event)) {
    autoSyncDelta->GetValue().ToLong(&MainFrame::settings.autoSyncSeconds);
    scrollSpeed->GetValue().ToInt(&MainFrame::settings.scrollSpeed);
    MainFrame::settings.autoSyncOnOpen = autoSync->GetValue();
    MainFrame::settings.exitPromptUnpushed = promptUnpushed->GetValue();
    MainFrame::settings.initGitOnOpen = initGitOnOpen->GetValue();

    Hide();
}

SettingsFrame::SettingsFrame(wxWindow* parent)
: wxFrame { parent, wxID_ANY, "Settings" } {
    SetClientSize(400, 185);
    CenterOnScreen();

    autoSyncLabel = new wxStaticText { this, wxID_ANY, "Auto sync delay in seconds (0 for never):", { 10, 10 } };
    autoSyncLabel->Wrap(180);

    scrollSpeedLabel = new wxStaticText { this, wxID_ANY, "Scroll speed:", { 10, 60 } };
    scrollSpeedLabel->Wrap(180);

    autoSyncDelta  = new wxTextCtrl { this, wxID_ANY, std::to_string(MainFrame::settings.autoSyncSeconds), { 220, 10 }, buttonSize };
    scrollSpeed    = new wxTextCtrl { this, wxID_ANY, std::to_string(MainFrame::settings.scrollSpeed), { 220, 50 }, buttonSize };
    autoSync       = new wxCheckBox { this, wxID_ANY, "Sync files on open", { 10, 90 } };
    promptUnpushed = new wxCheckBox { this, wxID_ANY, "Prompt on attempting to exit unpushed", { 10, 120 } };
    initGitOnOpen  = new wxCheckBox { this, wxID_ANY, "Initialize git on open", { 10, 150 } };

    autoSync->SetValue(MainFrame::settings.autoSyncOnOpen);
    promptUnpushed->SetValue(MainFrame::settings.exitPromptUnpushed);
    initGitOnOpen->SetValue(MainFrame::settings.initGitOnOpen);

    Bind(wxEVT_CLOSE_WINDOW, &SettingsFrame::onCloseEvent, this);
}
