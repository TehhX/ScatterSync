#include <ManifestManip.hpp>

#include <MainFrame.hpp>
#include <UserFileControl.hpp>

#include <cstdint>

constexpr u_char autoSyncOnOpenMask { 0b100 };
constexpr u_char promptUnpushedMask { 0b010 };
constexpr u_char initGitOnOpenMask  { 0b001 };

constexpr int SCROLL { 1 };
constexpr int IDENT  { 2 };

constexpr const char* const CLOUD { "scatterSyncCloud.bin" };
constexpr const char* const LOCAL { "scatterSyncLocal.bin" };

constexpr bool READ  {  true };
constexpr bool WRITE { false };

void ManifestManip::openFile(std::string name, bool fsType) {
    if (fileStream.is_open())
        closeFile();

    if (!UserFileControl::exists(name))
        throw ManiManiErr("One of the manifest files does not exist.", ManiManiErr::FAIL_READ);

    if (fsType == READ)
        fileStream.open(name, std::fstream::in | std::fstream::binary);
    else //    == WRITE
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

u_short ManifestManip::readIntegral(int bytes) {
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
    auto iter { userFileInfo.find(ident) };

    if (iter == userFileInfo.end())
        throw ManiManiErr(std::string { "Requested ident " } + std::to_string(ident) + std::string { " does not exist." }, ManiManiErr::FAIL_ACCESS);

    else
        return iter->second;
}

void ManifestManip::readCloud() {
    openFile(CLOUD, READ);

    MainFrame::settings.scrollSpeed = readIntegral(SCROLL);

    { // These bools use the same byte, different bits
        char in;
        fileStream >> in;

        MainFrame::settings.autoSyncOnOpen     = in & autoSyncOnOpenMask;
        MainFrame::settings.exitPromptUnpushed = in & promptUnpushedMask;
        MainFrame::settings.initGitOnOpen      = in & initGitOnOpenMask;
    }

    while (fileStream.peek() != EOF) {
        Ident          id { readIntegral(IDENT) };
        FileName fileName { readVariableLen() };
        GenName   genName { readVariableLen() };

        userFileInfo.insert({ id, { genName, "", fileName }});
    }
}

void ManifestManip::readLocal() {
    openFile(LOCAL, READ);

    while (fileStream.peek() != EOF) {
        Ident    ident { readIntegral(IDENT) };
        LocalDir dir   { readVariableLen() };

        // If cloud bin has changed and local hasn't, idents will be mismatched. This checks for it, and simply skips the file if it doesn't exist in cloud.bin.
        try {
            localDirOf(ident) = dir;
        } catch (const ManiManiErr& mme) {
            if (mme.errCode != ManiManiErr::FAIL_ACCESS)
                throw;
        }
    }
}

void ManifestManip::writeVariableLen(std::string_view value) {
    for (size_t i { 0 }; i < value.length(); i++)
        fileStream << value[i];

    fileStream << '\0';
}

void ManifestManip::writeIntegral(u_long value, int bytes) {
    fileStream << SC(u_char, value & 0x00'00'00'FF);

    if (SC(u_char, bytes) > 1)
        fileStream << SC(u_char, (value & 0x00'00'FF'00) >> byteW);
}

void ManifestManip::writeCloud() {
    openFile(CLOUD, WRITE);

    writeIntegral(MainFrame::settings.scrollSpeed, SCROLL);

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

        writeIntegral(ident, IDENT);
        writeVariableLen(fileNameOf(ident));
        writeVariableLen(genericNameOf(ident));
        UserFileControl::registerNew(ident);
    }
}

void ManifestManip::writeLocal() {
    openFile(LOCAL, WRITE);

    for (auto iter { userFileInfo.begin() }; iter != userFileInfo.end(); iter++) {
        Ident ident { iter->first };

        writeIntegral(ident, IDENT);
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
        newIdent = 0;

    userFileInfo.insert({ newIdent, { "Generic Name Here", "local/path/", "fileName.txt" }});
    UserFileControl::registerNew(newIdent);

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
