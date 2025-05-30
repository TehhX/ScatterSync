#include <GitControl.hpp>

#include <UserFileControl.hpp>

#include <cstdlib>
#include <iostream>

// Neuter Git calls to stop Git from controlling repositories unintentionally.
#define NEUTER_GIT 0

void GitControl::init() {
#if !NEUTER_GIT
    if (isActive)
        throw GitCtrlErr("Git is already initialized.", GitCtrlErr::BAD_INIT);

    if (system("git --version"))
        throw GitCtrlErr("Git is not installed. Please install and retry init.", GitCtrlErr::FAIL_GITEXEC);

    else if (system("git status"))
        throw GitCtrlErr("Scatter Sync executable is not inside repository folder. Please fix and retry initialization.", GitCtrlErr::FAIL_GITEXEC);
#endif
    isActive = true;
    isPushed = true;
}

void GitControl::pull() {
#if !NEUTER_GIT
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (system("git stash") || system("git pull"))
        throw GitCtrlErr("Pull failed.", GitCtrlErr::FAIL_MANIP);

    // Returns error code even if there is nothing to pop, so no error throwing.
    system("git stash pop");
#endif
}

void GitControl::push(bool warnNotInRepo) {
#if !NEUTER_GIT
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    system("git add .");
    system("git commit -m \"Automated ScatterSync commit.\"");

    if (system("git push"))
        throw GitCtrlErr("Push failed.", GitCtrlErr::FAIL_MANIP);
#endif
    isPushed = true;
}

void GitControl::sync(bool warnNotInRepo) {
#if !NEUTER_GIT
    // Should allow either in repo or locally untracked, not just in repo
    if (warnNotInRepo && UserFileControl::areAnyStatus(UserFileControl::Status::IN_LOCAL))
        throw GitCtrlErr("Some files are not inside the repository. Proceed anyway?", GitCtrlErr::SOME_OUTSIDE);
#endif
    pull();
    push(warnNotInRepo);
}

void GitControl::setEdited() {
#if !NEUTER_GIT
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);
#endif
    isPushed = false;
}

void GitControl::resetChanges() {
#if !NEUTER_GIT
    if (!isActive)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (system("git restore --staged .") || system("git restore ."))
        throw GitCtrlErr("Couldn't reset changes.", GitCtrlErr::FAIL_MANIP);
#endif
}

void GitControl::exitGitCtrl(bool throwErrors) {
#if !NEUTER_GIT
    if (!isActive && throwErrors)
        throw GitCtrlErr("GitControl is not initialized.", GitCtrlErr::BAD_INIT);

    if (!isPushed && throwErrors)
        throw GitCtrlErr("Are you sure you want to exit ScatterSync without pushing saved changes?", GitCtrlErr::UNPUSHED_EXIT);
#endif
    isActive = false;
}

GitControl::~GitControl() {
    if (isActive) {
        std::cerr << "GitControl destructed before calling \"GitControl& exitGitCtrl()\".\n";
        exit(-1);
    }
}
