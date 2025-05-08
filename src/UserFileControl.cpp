#include <UserFileControl.hpp>

#include <ManifestManip.hpp>

#include <filesystem>

namespace fsys = std::filesystem;

void UserFileControl::moveFile(std::string_view src, std::string_view dest) {
    if (!fsys::exists(src))
        throw UserFileErr("File does not exist.", UserFileErr::FAIL_MOVE);

    fsys::rename(src, dest);
}

void UserFileControl::moveToRepo(u_llong uniqueIdent) {
    
}

void UserFileControl::moveFromRepo(u_llong uniqueIdent) {

}
