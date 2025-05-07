#include <ManifestManip.hpp>

void ManifestManip::openFile(const char* name) {
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
