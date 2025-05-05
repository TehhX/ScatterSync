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

    const ErrCode errorCode;

    GitCtrlErr(const std::string& message, ErrCode errorCode);
};

class GitControl {
    bool isActive { false };
    bool isPushed { false };

public:
    GitControl() = default;

    GitControl& init();

    GitControl& pull();
    GitControl& push();
    GitControl& setEdited();
    GitControl& resetChanges();

    GitControl& exitGitCtrl(bool warnUnpushed = true); // Always warn unpushed unless user explicitly overrides warning.

    ~GitControl();
};
