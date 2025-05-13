#pragma once

#include <ScatterSyncDefs.hpp>

#include <stdexcept>
#include <string>

class GitCtrlErr : public std::runtime_error {
public:
    enum ErrCode : u_char {
        FAIL_GITEXEC,
        FAIL_MANIP,
        UNPUSHED_EXIT,
        BAD_INIT
    };

    const ErrCode errCode;

    inline GitCtrlErr(const std::string& mssg, ErrCode errCode)
    : std::runtime_error { mssg }, errCode { errCode } {}
};

class GitControl {
    bool isActive { false };
    bool isPushed;

public:
    GitControl() = default;

    void init();

    void pull();
    void push();
    void setEdited();
    void resetChanges();

    void exitGitCtrl(bool throwErrors = true); // Always warn unpushed unless user explicitly overrides warning.

    ~GitControl();
};
