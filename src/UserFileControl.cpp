#include <UserFileControl.hpp>

#include <ManifestManip.hpp>

#include <filesystem>

namespace fsys = std::filesystem;

void UserFileControl::moveFile(size_t index, bool inToOut) {
    fsys::path outsideRepo { ManifestManip::localPathOf(index) };
    fsys::path insideRepo  { ManifestManip::fileNameOf(index) };

    if (!exists(inToOut ? insideRepo : outsideRepo))
        throw UserFileErr("File does not exist.", UserFileErr::FAIL_MOVE);

    fsys::rename(inToOut ? insideRepo : outsideRepo, inToOut ? outsideRepo : insideRepo);
}

// A bit redundant, might remove later
bool UserFileControl::exists(std::string_view fileName) {
    return fsys::exists(fileName);
}
