#pragma once

#include <ScatterSyncDefs.hpp>

#include <stdexcept>
#include <string>

class GitCtrlErr : public std::runtime_error {
public:
    enum ErrCode : u_char {
        NOT_INSTALLED,
        FAIL_PULL,
        FAIL_PUSH,
        OUTSIDE_REPO,
        UNPUSHED_EXIT,
        UNINITIALIZED,
        DOUBLE_INIT
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

    GitControl& exitGitCtrl();

    ~GitControl();
};
