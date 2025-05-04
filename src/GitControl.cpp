#include <GitControl.hpp>

#include <cstdlib>
#include <iostream>

GitCtrlErr::GitCtrlErr(const std::string& message, ErrCode errorCode)
: std::runtime_error { message }, errorCode { errorCode } {}

GitControl& GitControl::init() {
    if (isActive)
        throw GitCtrlErr("Attempted to init twice without exiting.", GitCtrlErr::BAD_INIT);

    if (system("git --version"))
        throw GitCtrlErr("Git is not installed.", GitCtrlErr::FAIL_GITEXEC);

    else if (system("git status"))
        throw GitCtrlErr("Scatter Sync executable is not inside repository folder.", GitCtrlErr::FAIL_GITEXEC);

    isActive = true;

    return *this;
}

GitControl& GitControl::pull() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (system("git pull"))
        throw GitCtrlErr("Pull failed.", GitCtrlErr::FAIL_MANIP);
    else
        return *this;
}

GitControl& GitControl::push() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (system("git push"))
        throw GitCtrlErr("Push failed.", GitCtrlErr::FAIL_MANIP);

    isPushed = true;

    return *this;
}

GitControl& GitControl::setEdited() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (system("git add ."))
        throw GitCtrlErr("Could not add files to Git.",GitCtrlErr::FAIL_MANIP);

    isPushed = false;

    return *this;
}

GitControl& GitControl::resetChanges() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);
    
    if (system("git restore --staged .") || system("git restore ."))
        throw GitCtrlErr("Couldn't reset changes.", GitCtrlErr::FAIL_MANIP);

    return *this;
};

GitControl& GitControl::exitGitCtrl() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (!isPushed)
        throw GitCtrlErr("Attempted to exit program without pushing.", GitCtrlErr::UNPUSHED_EXIT);

    isActive = false;
    
    return *this;
}

GitControl::~GitControl() {
    if (isActive) {
        std::cerr << "GitControl destructed before calling \"GitControl& exitGitCtrl()\".\n";
        exit(-1);
    }
}
