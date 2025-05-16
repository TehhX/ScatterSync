#include <ManifestManip.hpp>

#include <cstdint>

#include <MainFrame.hpp>
#include <UserFileControl.hpp>

constexpr u_char autoSyncOnOpenMask { 0b1000 };
constexpr u_char promptUnpushedMask { 0b0100 };
constexpr u_char initGitOnOpenMask  { 0b0010 };

void ManifestManip::openFile(std::string name, bool in) {
    if (fileStream.is_open())
        closeFile();

    if (!UserFileControl::exists(name))
        throw ManiManiErr("One of the manifest files does not exist.", ManiManiErr::FAIL_READ);

    if (in)
        fileStream.open(name, std::fstream::in | std::fstream::binary);
    else
        fileStream.open(name, std::fstream::out | std::fstream::trunc | std::fstream::binary);

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

    throw ManiManiErr("Reached end of file without closing string (Lacks null terminator).", ManiManiErr::FAIL_READ);
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

    return value;
}

ManifestManip::UFITuple& ManifestManip::get(Ident ident) {
    UFIMap::iterator iter { userFileInfo.find(ident) };

    if (iter == userFileInfo.end())
        throw ManiManiErr(std::string { "Requested ident " } + std::to_string(ident) + std::string { " does not exist." }, ManiManiErr::FAIL_ACCESS);

    else
        return iter->second;
}

void ManifestManip::readCloud() {
    openCloud(true);

    MainFrame::settings.scrollSpeed = readIntegral(ByteCount::SCROLL_SPEED);

    { // These bools use the same byte, different bits
        char in;
        fileStream >> in;

        MainFrame::settings.autoSyncOnOpen     = in & autoSyncOnOpenMask;
        MainFrame::settings.exitPromptUnpushed = in & promptUnpushedMask;
        MainFrame::settings.initGitOnOpen      = in & initGitOnOpenMask;
    }

    while (fileStream.peek() != EOF) {
        Ident          id { readIntegral(ByteCount::IDENT) };
        FileName fileName { readVariableLen() };
        GenName   genName { readVariableLen() };

        userFileInfo.insert({ id, { genName, "", fileName }});
    }
}

void ManifestManip::readLocal() {
    openLocal(true);

    while (fileStream.peek() != EOF) {
        Ident    ident { readIntegral(ByteCount::IDENT) };
        LocalDir dir   { readVariableLen() };

        localDirOf(ident) = dir;
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
}

void ManifestManip::writeCloud() {
    openCloud(false);

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
        Ident ident { iter->first };

        writeIntegral(ident, ByteCount::IDENT);
        writeVariableLen(fileNameOf(ident));
        writeVariableLen(genericNameOf(ident));
        UserFileControl::registerNew(ident);
    }
}

void ManifestManip::writeLocal() {
    openLocal(false);

    for (auto iter { userFileInfo.begin() }; iter != userFileInfo.end(); iter++) {
        Ident ident { iter->first };

        writeIntegral(ident, ByteCount::IDENT);
        writeVariableLen(localDirOf(ident));
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

    userFileInfo.insert({ newIdent, { "Generic Name Here", "local/path/", "fileName.txt" }});

    return newIdent;
}

void ManifestManip::removeFileElement(Ident ident) {
    // Erase returns number of erased elements, if it erased nothing throw error
    if (!userFileInfo.erase(ident))
        throw ManiManiErr("Attempted to remove nonexistent UFI.", ManiManiErr::FAIL_IDENT);
}

std::string ManifestManip::dirAndNameOf(Ident ident) {
    std::string directoryAndName { "" };
    directoryAndName.append(localDirOf(ident)).append(fileNameOf(ident));

    return directoryAndName;
}

void ManifestManip::readFiles() {
    if (userFileInfo.size() > 0)
        userFileInfo.clear();

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
