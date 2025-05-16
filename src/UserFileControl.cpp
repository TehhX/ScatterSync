#include <UserFileControl.hpp>

#include <ManifestManip.hpp>

#include <filesystem>

namespace fsys = std::filesystem;

const UserFileControl::Status& UserFileControl::searchForAndAssign(ManifestManip::Ident ident) {
    Status fileStat;

    if (ManifestManip::localDirOf(ident) == "")
        fileStat = Status::LOCALLY_UNTRACKED;
    else if (exists(ManifestManip::fileNameOf(ident)))
        fileStat = Status::IN_REPO;
    else if (exists(ManifestManip::dirAndNameOf(ident)))
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

void UserFileControl::removeFromArr(ManifestManip::Ident ident) {
    // Erase returns number of erased elements, if it erased nothing throw error
    if (!statusArr.erase(ident))
        throw UserFileErr("Attempted to remove nonexistent Status.", UserFileErr::INVALID_IDENT);
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
    // Should implicit cast from "Status&" to "const Status&" for encapsulation
    return getStatusMutable(ident);
}

void UserFileControl::takeAction(ManifestManip::Ident ident, Action action) {
    // Ptr instead of ref due to scope issues (can't initialize ref to assign later)
    const Status* currentStatus;

    // If the passed ident doesn't exist, try to register. If registering fails i.e. ident doesn't exist in UFIMap, rethrow error. Hopefully.
    try {
        currentStatus = &getStatus(ident);
    }
    catch (const UserFileErr& ufe) {
        if (ufe.errCode == UserFileErr::INVALID_IDENT) {
            try {
                registerNew(ident);
                takeAction(ident, action);
            } catch (const ManiManiErr& mme) {
                throw;
            }
        }
        else
            throw;
    }

    if (action != Action::UNTRACK && *currentStatus == Status::MISSING && searchForAndAssign(ident) == Status::MISSING)
        throw UserFileErr(std::string { "Couldn't find specified file: " } + ManifestManip::genericNameOf(ident), UserFileErr::ACTION_ON_MISSING);

    // If untracked, and no change to track it, and move action
    if (*currentStatus == Status::LOCALLY_UNTRACKED && searchForAndAssign(ident) == Status::LOCALLY_UNTRACKED && (action == Action::MOVE_TO_LOCAL || action == Action::MOVE_TO_REPO))
        throw UserFileErr("Attempted to move untracked file.", UserFileErr::MOVE_ON_UNTRACKED);

    // If attempted action already matches status
    if ((*currentStatus == Status::IN_REPO && action == Action::MOVE_TO_REPO ) || (*currentStatus == Status::IN_LOCAL && action == Action::MOVE_TO_LOCAL))
        return;

    switch (action) {
    case Action::MOVE_TO_REPO:
        fsys::rename(ManifestManip::dirAndNameOf(ident), ManifestManip::fileNameOf(ident));
        getStatusMutable(ident) = Status::IN_REPO;
        break;

    case Action::MOVE_TO_LOCAL:
        fsys::rename(ManifestManip::fileNameOf(ident), ManifestManip::dirAndNameOf(ident));
        getStatusMutable(ident) = Status::IN_LOCAL;
        break;

    case Action::UNTRACK:
        switch(*currentStatus) {
        default:
            break;

        case Status::IN_REPO:
            takeAction(ident, Action::MOVE_TO_LOCAL);
            break;

        case Status::LOCALLY_UNTRACKED:
            fsys::remove(ManifestManip::fileNameOf(ident));
            break;
        }

        ManifestManip::removeFileElement(ident);
        removeFromArr(ident);
    }
}

void UserFileControl::takeActionsForEach(Action action) {
    MANI_FOR_EACH(
        try {
            takeAction(ident, action);
        } catch (const UserFileErr& ufe) {
            // If attempting to move an untracked file: ignore and go next. Elif: rethrow it.
            if (ufe.errCode != UserFileErr::MOVE_ON_UNTRACKED)
                throw;
        }
    )
}

bool UserFileControl::exists(std::string_view name) {
    return fsys::exists(name);
}

const UserFileControl::Status& UserFileControl::registerNew(ManifestManip::Ident ident) {
    return searchForAndAssign(ident);
}

bool UserFileControl::areAnyStatus(Status checkAgainst) {
    for (auto iter { statusArr.begin() }; iter != statusArr.end(); iter++)
        if (iter->second == checkAgainst)
            return true;

    return false;
}
