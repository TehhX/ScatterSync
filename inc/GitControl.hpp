#pragma once

#include <ScatterSyncDefs.hpp>
#include <ScatterSyncErr.hpp>

#include <string>

class GitCtrlErr : public ScatterSyncErr {
public:
    enum ErrCode : u_char {
        FAIL_GITEXEC,
        FAIL_MANIP,
        UNPUSHED_EXIT,
        BAD_INIT,
        SOME_OUTSIDE
    };

    inline GitCtrlErr(const std::string& mssg, ErrCode errCode)
    : ScatterSyncErr { mssg, SC(u_char, errCode) } {}
};

class GitControl {
    bool isActive { false };
    bool isPushed;

public:
    GitControl() = default;

    void init();

    void pull();
    void push(bool warnNotInRepo = true);
    void sync(bool warnNotInRepo = true);

    void setEdited();
    void resetChanges();

    void exitGitCtrl(bool throwErrors = true); // Always warn unpushed unless user explicitly overrides warning.

    ~GitControl();
};
