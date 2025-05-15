#include <SettingsFrame.hpp>

#include <MainFrame.hpp>
#include <ScatterSyncDefs.hpp>

void SettingsFrame::onCloseEvent(wxCloseEvent& WXUNUSED(event)) {
    int scrollInt { std::stoi(scrollSpeed->GetValue().ToStdString()) };
    if (scrollInt < 0 || scrollInt > 255) {
        POPUP("Scroll speed must be between 0 and 255.")
        return;
    }

    MainFrame::settings.scrollSpeed        = SC(u_char, scrollInt);
    MainFrame::settings.autoSyncOnOpen     = autoSync->GetValue();
    MainFrame::settings.exitPromptUnpushed = promptUnpushed->GetValue();
    MainFrame::settings.initGitOnOpen      = initGitOnOpen->GetValue();

    Hide();
}

SettingsFrame::SettingsFrame(wxWindow* parent)
: wxFrame { parent, wxID_ANY, "Settings" } {
    SetClientSize(400, 185);
    CenterOnScreen();

    scrollSpeedLabel = new wxStaticText { this, wxID_ANY, "Scroll speed:", { 10, 20 } };
    scrollSpeedLabel->Wrap(180);

    scrollSpeed    = new wxTextCtrl { this, wxID_ANY, std::to_string(MainFrame::settings.scrollSpeed), { 220, 10 }, buttonSize };
    autoSync       = new wxCheckBox { this, wxID_ANY, "Sync files on open", { 10, 90 } };
    promptUnpushed = new wxCheckBox { this, wxID_ANY, "Prompt on attempting to exit unpushed", { 10, 120 } };
    initGitOnOpen  = new wxCheckBox { this, wxID_ANY, "Initialize git on open", { 10, 150 } };

    autoSync->SetValue(MainFrame::settings.autoSyncOnOpen);
    promptUnpushed->SetValue(MainFrame::settings.exitPromptUnpushed);
    initGitOnOpen->SetValue(MainFrame::settings.initGitOnOpen);

    Bind(wxEVT_CLOSE_WINDOW, &SettingsFrame::onCloseEvent, this);
}
