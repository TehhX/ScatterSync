#include <UserFileControl.hpp>

#include <ManifestManip.hpp>

#include <filesystem>

namespace fsys = std::filesystem;

const UserFileControl::Status& UserFileControl::searchForAndAssign(ManifestManip::Ident ident) {
    Status fileStat;

    if (exists(ManifestManip::fileNameOf(ident)))
        fileStat = Status::IN_REPO;
    else if (exists(ManifestManip::localPathOf(ident)))
        fileStat = Status::IN_LOCAL;
    else
        fileStat = Status::MISSING;

    auto reqIter { statusArr.find(ident) };
    if (reqIter != statusArr.end())
        reqIter->second = fileStat;
    else
        statusArr.insert({ ident, fileStat });

    return getStatus(ident);
}

UserFileControl::Status& UserFileControl::getStatusMutable(ManifestManip::Ident ident) {
    auto reqIter { statusArr.find(ident) };

    if (reqIter != statusArr.end())
        return reqIter->second;
    else
        throw UserFileErr("Couldn't find requested status ident.", UserFileErr::INVALID_IDENT);
}

void UserFileControl::init() {
    if (active)
        throw UserFileErr("Attempted to initialize UserFileControl twice.", UserFileErr::DOUBLE_INIT);

    bool anyMissing { false };

    MANI_FOR_EACH(searchForAndAssign(ident);)

    active = true;

    if (anyMissing)
        throw UserFileErr("One or more files are missing.", UserFileErr::INIT_MISSING);
}

const UserFileControl::Status& UserFileControl::getStatus(ManifestManip::Ident ident) {
    const Status& constRefToStat { getStatusMutable(ident) };
    return constRefToStat;
}

void UserFileControl::takeAction(ManifestManip::Ident ident, Action action) {
    if (getStatus(ident) == Status::MISSING && searchForAndAssign(ident) == Status::MISSING)
        throw UserFileErr("Couldn't find specified file.", UserFileErr::ACTION_ON_MISSING);

    // If attempted action already matches status
    if ((getStatus(ident) == Status::IN_REPO && action == Action::MOVE_TO_REPO ) || (getStatus(ident) == Status::IN_LOCAL && action == Action::MOVE_TO_LOCAL))
        return;

    switch (action) {
    case Action::MOVE_TO_REPO:
        fsys::rename(ManifestManip::localPathOf(ident), ManifestManip::fileNameOf(ident));
        getStatusMutable(ident) = Status::IN_REPO;
        break;

    case Action::MOVE_TO_LOCAL:        
        fsys::rename(ManifestManip::fileNameOf(ident), ManifestManip::localPathOf(ident));
        getStatusMutable(ident) = Status::IN_LOCAL;
        break;

    case Action::REMOVE:
        if (getStatus(ident) == Status::IN_REPO)
            fsys::remove(ManifestManip::fileNameOf(ident));

        else // Status::IN_LOCAL
            fsys::remove(ManifestManip::localPathOf(ident));

        getStatusMutable(ident) = Status::MISSING;
    }
}

void UserFileControl::takeActionsForEach(Action action) {
    MANI_FOR_EACH(takeAction(ident, action);)
}

bool UserFileControl::exists(std::string_view name) {
    return fsys::exists(name);
}

const UserFileControl::Status& UserFileControl::registerNew(ManifestManip::Ident ident) {
    return searchForAndAssign(ident);
}

bool UserFileControl::areAnyNotStatus(Status checkAgainst) {
    for (auto iter { statusArr.begin() }; iter != statusArr.end(); iter++)
        if (iter->second != checkAgainst)
            return true;

    return false;
}
