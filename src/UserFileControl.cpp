#include <UserFileControl.hpp>

#include <ManifestManip.hpp>

#include <filesystem>

namespace fsys = std::filesystem;

void UserFileControl::init() {
    if (active)
        throw UserFileErr("Attempted to initialize UserFileControl twice.", UserFileErr::DOUBLE_INIT);

    bool anyMissing { false };

    statusArr.reserve(ManifestManip::size());
    for (size_t i { 0 }; i < ManifestManip::size(); i++) {
        if (fsys::exists(ManifestManip::fileNameOf(i)))
            statusArr.push_back(Status::IN_REPO);

        else if (fsys::exists(ManifestManip::localPathOf(i)))
            statusArr.push_back(Status::IN_LOCAL);

        else {
            statusArr.push_back(Status::MISSING);
            anyMissing = true;
        }
    }

    active = true;

    if (anyMissing)
        throw UserFileErr("One or more files are missing.", UserFileErr::INIT_MISSING);
}

void UserFileControl::takeAction(size_t index, Action action) {
    if (getStatus(index) == Status::MISSING)
        throw UserFileErr("Couldn't find specified file.", UserFileErr::ACTION_ON_MISSING);

    switch (action) {
    case Action::MOVE_TO_REPO:
        fsys::rename(ManifestManip::localPathOf(index), ManifestManip::fileNameOf(index));
        break;

    case Action::MOVE_TO_LOCAL:        
        fsys::rename(ManifestManip::fileNameOf(index), ManifestManip::localPathOf(index));
        break;

    case Action::DELETE:
        if (getStatus(index) == Status::IN_REPO)
            fsys::remove(ManifestManip::fileNameOf(index));

        else // Status::IN_LOCAL
            fsys::remove(ManifestManip::localPathOf(index));
    }
}

bool UserFileControl::exists(std::string_view name) {
    return fsys::exists(name);
}
