#include <ManifestManip.hpp>

#include <cstdint>

#include <MainFrame.hpp>

void ManifestManip::openFile(std::string name) {
    if (fileStream.is_open())
        closeFile();

    fileStream.open(name);

    if (fileStream.fail())
        throw ManiManiErr("Couldn't open file.", ManiManiErr::FAIL_OPEN);
}

// TODO: Untested
std::string ManifestManip::readVariableLen() {
    std::string value {};
    char input {};

    WH_EVER {
        fileStream >> input;

        // Variable length strings inside binary files must end in '\0' null terminator.
        if (input == '\0')
            return value;

        value.append(std::string { input });
    }
}

// TODO: Untested
u_long ManifestManip::readIntegral(ByteCount bytes) {
    constexpr static u_char byteW { 8 }; // Bits/Byte width

    u_llong value { 0 };
    u_char input  { 0 };

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

// TODO: Untested
void ManifestManip::readCloud() {
    openCloud();

    // Read auto-sync interval

    // Read scroll-speed
    
    // Read auto-sync on open

    // Read prompt on unpuhsed exit

    // Until file is exhausted:

        // Read generic name for file n

        // Read ident for file n
}

// TODO: Untested
void ManifestManip::readLocal() {
    openLocal();

    // Until file is exhausted:

        // Read ident for file n

        // Read local path for file n
}

// TODO: Untested
void ManifestManip::writeVariableLen(std::string_view value) {
    for (size_t i { 0 }; i < value.length(); i++)
        fileStream << value[i];

    fileStream << '\0';
}

// TODO: Untested
void ManifestManip::writeIntegral(u_long value, ByteCount bytes) {
    fileStream << SC(u_char, value & 0x00'00'00'FF);

    if (SC(u_char, bytes) > 1)
        fileStream << SC(u_char, (value & 0x00'00'FF'00) >> 8);

    if (SC(u_char, bytes) > 2)
        fileStream << SC(u_char, (value & 0x00'FF'00'00) >> 16);

    if (SC(u_char, bytes) > 3)
        fileStream << SC(u_char, (value & 0xFF'00'00'00) >> 24);
}

// TODO: Untested
void ManifestManip::writeCloud() {
    openCloud();

    writeIntegral(MainFrame::settings.autoSyncSeconds, ByteCount::FOUR);
    writeIntegral(MainFrame::settings.scrollSpeed, ByteCount::TWO);

    /* Write both auto sync on open and prompt on unpushed exit booleans */ {
        static constexpr u_char autoSyncOnOpenMask { 0b00'00'00'01 };
        static constexpr u_char promptUnpushedMask { 0b00'00'00'10 };

        // This is probably comically wrong, test it later
        auto out { SC(u_char,
            (MainFrame::settings.autoSyncOnOpen     ? autoSyncOnOpenMask : 0) |
            (MainFrame::settings.exitPromptUnpushed ? promptUnpushedMask : 0)
        )};

        fileStream << out;
    }

    for (auto i { getBegin() }; i != getEnd(); i++) {
        writeVariableLen(genNameOf(i->first));
        writeIntegral(i->first, ByteCount::FOUR);
    }
}

// TODO: Untested
void ManifestManip::writeLocal() {
    openLocal();

    for (auto i { getBegin() }; i != getEnd(); i++) {
        writeIntegral(i->first, ByteCount::FOUR);
        writeVariableLen(localPathOf(i->first));
    }
}

// TODO: Untested
void ManifestManip::tryAccess(u_long uniqueIdent) {
    if (userFileInfo.find(uniqueIdent) == userFileInfo.end())
        throw ManiManiErr("Identifier does not exist.", ManiManiErr::FAIL_ACCESS);
}

std::string& ManifestManip::genNameOf(u_long uniqueIdent) {
    tryAccess(uniqueIdent);

    return userFileInfo[uniqueIdent].first;
}

std::string& ManifestManip::localPathOf(u_long uniqueIdent) {
    tryAccess(uniqueIdent);

    return userFileInfo[uniqueIdent].second;
}

// TODO: Untested
std::string ManifestManip::fileNameOf(u_long uniqueIdent) {
    tryAccess(uniqueIdent);

    std::string name { localPathOf(uniqueIdent) };

    size_t i { name.length() };
    while (--i != SIZE_MAX)
        if (name[i] == '/')
            return name.substr(i + 1);

    // Only gets to this point if '/' is not found.
    throw ManiManiErr("Requested file path is invalid", ManiManiErr::FAIL_IDENT);
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
