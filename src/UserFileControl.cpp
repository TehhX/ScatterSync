#include <UserFileControl.hpp>

#include <ManifestManip.hpp>

#include <filesystem>

namespace fsys = std::filesystem;

const UserFileControl::Status& UserFileControl::searchForAndAssign(size_t index) {
    Status fileStat;

    if (fsys::exists(ManifestManip::fileNameOf(index)))
        fileStat = Status::IN_REPO;
    else if (fsys::exists(ManifestManip::localPathOf(index)))
        fileStat = Status::IN_LOCAL;
    else
        fileStat = Status::MISSING;

    if (index < size())
        statusArr[index] = fileStat;
    else
        statusArr.push_back(fileStat);

    return getStatus(index);
}

void UserFileControl::init() {
    if (active)
        throw UserFileErr("Attempted to initialize UserFileControl twice.", UserFileErr::DOUBLE_INIT);

    bool anyMissing { false };

    statusArr.reserve(ManifestManip::size());
    for (size_t i { 0 }; i < ManifestManip::size(); i++)
        searchForAndAssign(i);

    active = true;

    if (anyMissing)
        throw UserFileErr("One or more files are missing.", UserFileErr::INIT_MISSING);
}

void UserFileControl::takeAction(size_t index, Action action) {
    if (getStatus(index) == Status::MISSING && searchForAndAssign(index) == Status::MISSING)
        throw UserFileErr("Couldn't find specified file.", UserFileErr::ACTION_ON_MISSING);

    // If attempted action already matches status
    if ((statusArr[index] == Status::IN_REPO  && action == Action::MOVE_TO_REPO ) || (statusArr[index] == Status::IN_LOCAL && action == Action::MOVE_TO_LOCAL))
        return;

    switch (action) {
    case Action::MOVE_TO_REPO:
        fsys::rename(ManifestManip::localPathOf(index), ManifestManip::fileNameOf(index));
        statusArr[index] = Status::IN_REPO;
        break;

    case Action::MOVE_TO_LOCAL:        
        fsys::rename(ManifestManip::fileNameOf(index), ManifestManip::localPathOf(index));
        statusArr[index] = Status::IN_LOCAL;
        break;

    case Action::REMOVE:
        if (getStatus(index) == Status::IN_REPO)
            fsys::remove(ManifestManip::fileNameOf(index));

        else // Status::IN_LOCAL
            fsys::remove(ManifestManip::localPathOf(index));

        statusArr[index] = Status::MISSING;
    }
}

void UserFileControl::takeActionsAll(Action action) {
    for (size_t i { 0 }; i < ManifestManip::size(); i++)
        takeAction(i, action);
}

bool UserFileControl::exists(std::string_view name) {
    return fsys::exists(name);
}

const UserFileControl::Status& UserFileControl::registerNew(size_t index) {
    return searchForAndAssign(index);
}
