#pragma once

#include <ScatterSyncDefs.hpp>

#include <exception>
#include <string_view>

enum class GitCtrlErrCode : u_char {
    FAIL_PULL,
    FAIL_PUSH,
    OUTSIDE_REPO,
    UNPUSHED_EXIT
};

class GitCtrlErr : public std::exception {
    GitCtrlErrCode errorCode;
    std::string_view message;

public:
    GitCtrlErr(GitCtrlErrCode errorCode, std::string_view message);
};

class GitControl {
    bool isPushed { false };

public:
    GitControl();

    GitControl& pull();
    GitControl& push();

    void exitGitCtrl();
};
