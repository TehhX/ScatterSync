#include <GitControl.hpp>

#include <cstdlib>
#include <iostream>

GitCtrlErr::GitCtrlErr(GitCtrlErrCode errorCode, std::string_view message) {
    // TODO
}

GitControl::GitControl() {
    // Will exit if git isn't installed
    if (system("git --version")) {
        std::cerr << "Git not installed.\n";
        exit(-1);
    }
    // Will exit if executable is not inside repo folder
    else if (system("git status")) {
        std::cerr << "Exec not next to repository.\n";
        exit(-1);
    }
}

GitControl& GitControl::pull() {
    if (system("git pull")) {
        // TODO
    }

    return *this;
}

GitControl& GitControl::push() {
    if (system("git push")) {
        // TODO
    }

    return *this;
}

void GitControl::exitGitCtrl() {
    // TODO: If repo was not pushed, throw error
}
