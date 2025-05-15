#include <UserFileControl.hpp>

#include <ManifestManip.hpp>

#include <filesystem>

namespace fsys = std::filesystem;

const UserFileControl::Status& UserFileControl::searchForAndAssign(ManifestManip::Ident ident) {
    Status fileStat;

    if (ManifestManip::fileNameOf(ident) == "")
        fileStat = Status::UNTRACKED;
    else if (exists(ManifestManip::fileNameOf(ident)))
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

    // If untracked, and no change to track it, and move action
    if (getStatus(ident) == Status::UNTRACKED && searchForAndAssign(ident) == Status::UNTRACKED && (action == Action::MOVE_TO_LOCAL || action == Action::MOVE_TO_REPO))
        throw UserFileErr("Attempted to move untracked file.", UserFileErr::MOVE_ON_UNTRACKED);

    // If attempted action already matches status
    if ((getStatus(ident) == Status::IN_REPO && action == Action::MOVE_TO_REPO ) || (getStatus(ident) == Status::IN_LOCAL && action == Action::MOVE_TO_LOCAL))
        return;

    if (action == Action::MOVE_TO_REPO) {
        fsys::rename(ManifestManip::localPathOf(ident), ManifestManip::fileNameOf(ident));
        getStatusMutable(ident) = Status::IN_REPO;
    }
    else { 
        fsys::rename(ManifestManip::fileNameOf(ident), ManifestManip::localPathOf(ident));
        getStatusMutable(ident) = Status::IN_LOCAL;
    }
}

void UserFileControl::takeActionsForEach(Action action) {
    MANI_FOR_EACH(
        try {
            takeAction(ident, action);
        } catch (const UserFileErr& ufe) {
            if (ufe.errCode != UserFileErr::MOVE_ON_UNTRACKED)
                throw ufe;
        }
    )
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
