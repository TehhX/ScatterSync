#include <GitControl.hpp>

#include <cstdlib>
#include <iostream>

/*
    TODO: Look into CreateProcess(...) from Windows.h when building for Windows to replace system(...). 
    Linux may also have a better alternative to system(...).

    Further: Look into wxProcess(...) and/or wxExecute(...)
*/

void GitControl::init() {
    if (isActive)
        throw GitCtrlErr("Git is already initialized.", GitCtrlErr::BAD_INIT);

    if (system("git --version"))
        throw GitCtrlErr("Git is not installed. Please install and retry init.", GitCtrlErr::FAIL_GITEXEC);

    else if (system("git status"))
        throw GitCtrlErr("Scatter Sync executable is not inside repository folder. Please fix and retry initialization.", GitCtrlErr::FAIL_GITEXEC);

    isActive = true;
    isPushed = false;
}

void GitControl::pull() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (system("git pull"))
        throw GitCtrlErr("Pull failed.", GitCtrlErr::FAIL_MANIP);
}

void GitControl::push() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (isPushed)
        throw GitCtrlErr("Already pushed.", GitCtrlErr::FAIL_MANIP);

    if (system("git push"))
        throw GitCtrlErr("Push failed.", GitCtrlErr::FAIL_MANIP);

    isPushed = true;
}

void GitControl::setEdited() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (system("git add ."))
        throw GitCtrlErr("Could not add files to Git.",GitCtrlErr::FAIL_MANIP);

    isPushed = false;
}

void GitControl::resetChanges() {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);
    
    if (system("git restore --staged .") || system("git restore ."))
        throw GitCtrlErr("Couldn't reset changes.", GitCtrlErr::FAIL_MANIP);
}

void GitControl::exitGitCtrl(bool warnUnpushed) {
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (!isPushed && warnUnpushed)
        throw GitCtrlErr("Are you sure you want to exit ScatterSync without pushing saved changes?", GitCtrlErr::UNPUSHED_EXIT);

    isActive = false;
}

GitControl::~GitControl() {
    if (isActive) {
        std::cerr << "GitControl destructed before calling \"GitControl& exitGitCtrl()\".\n";
        exit(-1);
    }
}
