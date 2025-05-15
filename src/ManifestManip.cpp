#include <ManifestManip.hpp>

#include <cstdint>

#include <MainFrame.hpp>
#include <UserFileControl.hpp>

constexpr u_char autoSyncOnOpenMask { 0b100 };
constexpr u_char promptUnpushedMask { 0b010 };
constexpr u_char initGitOnOpenMask  { 0b001 };

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

u_short ManifestManip::readIntegral(ByteCount bytes) {
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

ManifestManip::UFIPair& ManifestManip::get(Ident ident) {
    UFIMap::iterator iter { userFileInfo.find(ident) };

    if (iter == userFileInfo.end())
        throw ManiManiErr("Requested index does not exist.", ManiManiErr::FAIL_ACCESS);

    else
        return iter->second;
}

void ManifestManip::readCloud() {
    openCloud();

    MainFrame::settings.scrollSpeed = readIntegral(ByteCount::SCROLL_SPEED);

    { // These bools use the same byte, different bits
        char in;
        fileStream >> in;

        MainFrame::settings.autoSyncOnOpen     = in & autoSyncOnOpenMask;
        MainFrame::settings.exitPromptUnpushed = in & promptUnpushedMask;
        MainFrame::settings.initGitOnOpen      = in & initGitOnOpenMask;
    }

    while (fileStream.peek() != EOF) {
        Ident     id { readIntegral(ByteCount::IDENT) };
        GenName name { readVariableLen() };

        userFileInfo.insert({ id, { name, "NULL" } });
    }
}

void ManifestManip::readLocal() {
    openLocal();

    while (fileStream.peek() != EOF) {
        Ident       id { readIntegral(ByteCount::IDENT) };
        LocalPath path { readVariableLen() };

        localPathOf(id) = path;
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
        fileStream << SC(u_char, (value & 0x00'00'FF'00) >> byteW);

    if (SC(u_char, bytes) > 2)
        fileStream << SC(u_char, (value & 0x00'FF'00'00) >> byteW * 2);

    if (SC(u_char, bytes) > 3)
        fileStream << SC(u_char, (value & 0xFF'00'00'00) >> byteW * 3);
}

void ManifestManip::writeCloud() {
    openCloud();

    writeIntegral(MainFrame::settings.scrollSpeed, ByteCount::SCROLL_SPEED);

    { // These bools use the same byte, different bits
        auto out { SC(u_char,
            (MainFrame::settings.autoSyncOnOpen     ? autoSyncOnOpenMask : 0) |
            (MainFrame::settings.exitPromptUnpushed ? promptUnpushedMask : 0) |
            (MainFrame::settings.initGitOnOpen      ? initGitOnOpenMask  : 0)
        )};

        fileStream << out;
    }

    for (auto iter { userFileInfo.begin() }; iter != userFileInfo.end(); iter++) {
        writeIntegral(iter->first, ByteCount::IDENT);
        writeVariableLen(iter->second.first);
    }
}

void ManifestManip::writeLocal() {
    openLocal();

    for (auto iter { userFileInfo.begin() }; iter != userFileInfo.end(); iter++) {
        writeIntegral(iter->first, ByteCount::IDENT);
        writeVariableLen(iter->second.second);
    }
}

void ManifestManip::_forEach_(std::function<void(Ident ident)> func) {
    for (auto iter { userFileInfo.begin() }; iter != userFileInfo.end(); iter++)
        func(iter->first);
}

ManifestManip::Ident ManifestManip::createNewFileElement() {
    Ident newIdent;

    if (userFileInfo.size() > 0)
        newIdent = SC(Ident, (std::prev(userFileInfo.end())->first) + 1);
    else
        newIdent = 1;

    userFileInfo.insert({ newIdent, { "Generic Name Here", "local/path/to.file" }});

    return newIdent;
}

std::string& ManifestManip::genericNameOf(Ident ident) {
    return get(ident).first;
}

std::string& ManifestManip::localPathOf(Ident ident) {
    return get(ident).second;
}

std::string ManifestManip::fileNameOf(Ident ident) {
    std::string name { localPathOf(ident) };

    size_t i { name.length() };
    while (--i != SIZE_MAX) // Unsigned analogue of "--i >= 0".
        if (name[i] == '/')
            return name.substr(i + 1);

    // Only gets to this point if '/' is not found in path.
    throw ManiManiErr(std::string { "Requested file path: \"" } + name + std::string { "\"is invalid (lacks file name)." }, ManiManiErr::FAIL_ACCESS);
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
