#pragma once

#include <ScatterSyncDefs.hpp>
#include <ScatterSyncErr.hpp>

#include <string>
#include <fstream>
#include <stdexcept>
#include <map>
#include <tuple>
#include <functional>

#define MANI_FOR_EACH(EXEC) ManifestManip::_forEach_({[&](ManifestManip::Ident ident) -> void { EXEC }});

class ManiManiErr : public ScatterSyncErr {
public:
    enum ErrCode : u_char {
        FAIL_OPEN,
        FAIL_CLOSE,
        FAIL_READ,
        FAIL_WRITE,
        FAIL_ACCESS,
        FAIL_IDENT
    };

    inline ManiManiErr(const std::string& mssg, ErrCode errCode)
    : ScatterSyncErr { mssg, SC(u_char, errCode) } {}
};

// A class for reading, writing, and accessing static data from the manifest files.
class ManifestManip {
public:
    using Ident    = u_short;     // Unique identifer
    using GenName  = std::string; // Generic name
    using LocalDir = std::string; // Local directory
    using FileName = std::string; // File name and extension

    using UFITuple = std::tuple<GenName, LocalDir, FileName>; // A tuple containing the generic name, local directory and file name for a file
    using UFIMap   = std::map<Ident, UFITuple>;               // A map between unique identifiers and their information

private:
    static constexpr int byteW { 8 }; // Bits/Byte width

    // For each element, there is a generic name (0th), local directory (1st), and file name (2nd). Commonly referred to as UFI.
    static inline UFIMap userFileInfo {};

    static inline std::fstream fileStream {};

    static void openFile(std::string name, bool fsType);

    static std::string readVariableLen();
    static u_short readIntegral(int bytes);

    static UFITuple& get(Ident ident);

    static void readCloud();
    static void readLocal();

    static void writeVariableLen(std::string_view value);
    static void writeIntegral(u_long value, int bytes);

    static void writeCloud();
    static void writeLocal();

public:
    ManifestManip() = delete; // Entirely static class

    // Use MANI_FOR_EACH(EXEC) instead of calling this directly.
    static void _forEach_(std::function<void(Ident ident)> func);

    static Ident createNewFileElement();
    static void removeFileElement(Ident ident);

    static inline GenName& genericNameOf(Ident ident) { return std::get<0>(get(ident)); }
    static inline LocalDir& localDirOf(Ident ident)   { return std::get<1>(get(ident)); }
    static inline FileName& fileNameOf(Ident ident)   { return std::get<2>(get(ident)); }
    static std::string dirAndNameOf(Ident ident);

    static void readFiles();
    static void writeFiles();

    static void closeFile();
};
