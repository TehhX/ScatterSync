#include <ManifestManip.hpp>

#include <cstdint>

#include <MainFrame.hpp>
#include <UserFileControl.hpp>

constexpr u_char autoSyncOnOpenMask { 1 };
constexpr u_char promptUnpushedMask { 2 };

void ManifestManip::openFile(std::string name) {
    if (fileStream.is_open())
        closeFile();

    if (!UserFileControl::exists(name))
        throw ManiManiErr("One of the manifest files does not exist.", ManiManiErr::FAIL_READ);

    fileStream.open(name);
    fileStream >> std::noskipws;

    if (fileStream.fail())
        throw ManiManiErr("Couldn't open file.", ManiManiErr::FAIL_OPEN);
}

std::string ManifestManip::readVariableLen() {
    std::string value {};
    char input {};

    while (fileStream.peek() != EOF) {
        fileStream >> input;

        if (input == '\0') // Strings inside binary file end in '\0'.
            return value;
        else
            value.append({ input });
    }

    throw ManiManiErr("Reached end of file without closing string.", ManiManiErr::FAIL_READ);
}

u_long ManifestManip::readIntegral(ByteCount bytes) {
    constexpr static u_char byteW { 8 }; // Bits/Byte width

    u_long value { 0 };
    u_char input { 0 };

    fileStream >> input;
    value += SC(u_long, input);

    if (SC(u_char, bytes) > 1) {
        fileStream >> input;
        value += SC(u_long, input) << byteW;
    }

    if (SC(u_char, bytes) > 2) {
        fileStream >> input;
        value += SC(u_long, input) << byteW * 2;
    }

    if (SC(u_char, bytes) > 3) {
        fileStream >> input;
        value += SC(u_long, input) << byteW * 3;
    }

    return value;
}

void ManifestManip::readCloud() {
    openCloud();

    MainFrame::settings.autoSyncSeconds = readIntegral(ByteCount::LONG);
    MainFrame::settings.scrollSpeed = readIntegral(ByteCount::INT);

    { // Read both autoSyncOnOpen and exitPromptUnpushed
        char in;
        fileStream >> in;

        MainFrame::settings.autoSyncOnOpen = in & autoSyncOnOpenMask;
        MainFrame::settings.exitPromptUnpushed = in & promptUnpushedMask;
    }

    while (fileStream.peek() != EOF) {
        std::string genName { readVariableLen() };
        u_long ident        { readIntegral(ByteCount::IDENT) };

        userFileInfo.insert({ ident, { genName, "NULL" } });
    }
}

void ManifestManip::readLocal() {
    openLocal();

    while (fileStream.peek() != EOF) {
        u_long ident     { readIntegral(ByteCount::IDENT) };
        std::string path { readVariableLen() };

        // This only works if readLocal() is ALWAYS called after readCloud(), as this assumes map keys have already been created by readCloud().
        userFileInfo.find(ident)->second.second = path;
    }
}

void ManifestManip::writeVariableLen(std::string_view value) {
    for (size_t i { 0 }; i < value.length(); i++)
        fileStream << value[i];

    fileStream << '\0';
}

void ManifestManip::writeIntegral(u_long value, ByteCount bytes) {
    fileStream << SC(u_char, value & 0x00'00'00'FF);

    if (SC(u_char, bytes) > 1)
        fileStream << SC(u_char, (value & 0x00'00'FF'00) >> 8);

    if (SC(u_char, bytes) > 2)
        fileStream << SC(u_char, (value & 0x00'FF'00'00) >> 16);

    if (SC(u_char, bytes) > 3)
        fileStream << SC(u_char, (value & 0xFF'00'00'00) >> 24);
}

void ManifestManip::writeCloud() {
    openCloud();

    writeIntegral(MainFrame::settings.autoSyncSeconds, ByteCount::LONG);
    writeIntegral(MainFrame::settings.scrollSpeed, ByteCount::INT);

    { // Write both auto sync on open and prompt on unpushed exit booleans
        auto out { SC(u_char,
            (MainFrame::settings.autoSyncOnOpen     ? autoSyncOnOpenMask : 0) |
            (MainFrame::settings.exitPromptUnpushed ? promptUnpushedMask : 0)
        )};

        fileStream << out;
    }

    for (auto i { userFileInfo.begin() }; i != userFileInfo.end(); i++) {
        writeVariableLen(genNameOf(i->first));
        writeIntegral(i->first, ByteCount::U_LONG);
    }
}

void ManifestManip::writeLocal() {
    openLocal();

    for (auto i { userFileInfo.begin() }; i != userFileInfo.end(); i++) {
        writeIntegral(i->first, ByteCount::IDENT);
        writeVariableLen(localPathOf(i->first));
    }
}

ManifestManip::ufiPairType& ManifestManip::tryAccess(u_long uniqueIdent) {
    const ufiMapType::iterator mapIterator { userFileInfo.find(uniqueIdent) };

    if (mapIterator == userFileInfo.end())
        throw ManiManiErr("Identifier does not exist.", ManiManiErr::FAIL_ACCESS);
    else
        return mapIterator->second;
}

std::string& ManifestManip::genNameOf(u_long uniqueIdent) {
    return tryAccess(uniqueIdent).first;
}

std::string& ManifestManip::localPathOf(u_long uniqueIdent) {
    return tryAccess(uniqueIdent).second;
}

// TODO: Untested
std::string ManifestManip::fileNameOf(u_long uniqueIdent) {
    std::string name { localPathOf(uniqueIdent) };

    size_t i { name.length() };
    while (--i != SIZE_MAX)
        if (name[i] == '/')
            return name.substr(i + 1);

    throw ManiManiErr("Requested file path is invalid", ManiManiErr::FAIL_IDENT); // Only gets to this point if '/' is not found in path.
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
