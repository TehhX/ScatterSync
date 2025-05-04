#include <GitControl.hpp>

#include <cstdlib>
#include <iostream>

GitCtrlErr::GitCtrlErr(const std::string& message, ErrCode errorCode)
: std::runtime_error { message }, errorCode { errorCode } {}

GitControl& GitControl::init() {
    if (isActive)
        throw GitCtrlErr("Attempted to init twice without exiting.", GitCtrlErr::DOUBLE_INIT);

    if (system("git --version"))
        throw GitCtrlErr("Git is not installed.", GitCtrlErr::NOT_INSTALLED);

    else if (system("git status"))
        throw GitCtrlErr("Scatter Sync executable is not inside repository folder.", GitCtrlErr::OUTSIDE_REPO);

    isActive = true;

    return *this;
}

GitControl& GitControl::pull() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::UNINITIALIZED);

    if (system("git pull"))
        throw GitCtrlErr("Pull failed.", GitCtrlErr::FAIL_PULL);
    else
        return *this;
}

GitControl& GitControl::push() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::UNINITIALIZED);

    if (system("git add .") || system("git push"))
        throw GitCtrlErr("Push failed.", GitCtrlErr::FAIL_PUSH);
    else
        isPushed = true;

    return *this;
}

GitControl& GitControl::setEdited() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::UNINITIALIZED);

    isPushed = false;

    return *this;
}

GitControl& GitControl::exitGitCtrl() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::UNINITIALIZED);
    if (!isPushed)
        throw GitCtrlErr("Attempted to exit program without pushing.", GitCtrlErr::UNPUSHED_EXIT);

    isActive = false;
    
    return *this;
}

GitControl::~GitControl() {
    if (isActive) {
        std::cerr << "GitControl destructed before calling \"exitGitCtrl\".\n";
        exit(-1);
    }
}
