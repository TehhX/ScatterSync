#include <ManifestManip.hpp>

#include <iterator>

void ManifestManip::openFile(std::string name) {
    if (fileStream.is_open())
        closeFile();

    fileStream.open(name);

    if (fileStream.fail())
        throw ManiManiErr("Couldn't open file.", ManiManiErr::FAIL_OPEN);
}

void ManifestManip::readCloud() {
    openFile(fileNameCloud);

    // TODO: Read data
}

void ManifestManip::readLocal() {
    openFile(fileNameLocal);

    // TODO: Read data
}

void ManifestManip::writeCloud() {
    openFile(fileNameCloud);

    // TODO: Write data
}

void ManifestManip::writeLocal() {
    openFile(fileNameLocal);

    // TODO: Write data
}

void ManifestManip::tryAccess(u_llong uniqueIdent) {
    if (userFileInfo.find(uniqueIdent) == userFileInfo.end())
        throw ManiManiErr("Identifier does not exist.", ManiManiErr::FAIL_ACCESS);
}

std::string& ManifestManip::genNameOf(u_llong uniqueIdent) {
    tryAccess(uniqueIdent);

    return userFileInfo[uniqueIdent].first;
}

std::string& ManifestManip::localPathOf(u_llong uniqueIdent) {
    tryAccess(uniqueIdent);

    return userFileInfo[uniqueIdent].second;
}

// Untested within the program, but tested in isolation.
std::string ManifestManip::fileNameOf(u_llong uniqueIdent) {
    tryAccess(uniqueIdent);

    std::string name { localPathOf(uniqueIdent) };
    auto i { static_cast<int>(name.length() - 1) };
    while (i-- >= 0)
        if (name[i] == '/')
            return name.substr(i + 1);

    // Only gets to this point if '/' is not found.
    throw ManiManiErr("Requested file path is invalid", ManiManiErr::FAIL_NAME);
}

void ManifestManip::readFiles() {
    readCloud();
    readLocal();
}

void ManifestManip::writeFiles() {
    writeCloud();
    writeLocal();
}

void ManifestManip::closeFile() {
    if (fileStream.is_open())
        fileStream.close();

    if (fileStream.fail())
        throw ManiManiErr("Could not close file.", ManiManiErr::FAIL_CLOSE);
}
