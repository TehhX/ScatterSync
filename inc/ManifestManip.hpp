#pragma once

#include <ScatterSyncDefs.hpp>

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
    using Ident    = u_short;
    using GenName  = std::string;
    using LocalDir = std::string;
    using FileName = std::string;

    using UFITuple = std::tuple<GenName, LocalDir, FileName>;
    using UFIMap   = std::map<Ident, UFITuple>;

private:
    enum class ByteCount : u_char {
        SCROLL_SPEED = 1,
        IDENT        = 2
    };

    static constexpr u_char byteW { 8 }; // Bits/Byte width

    // For each element, there is a generic name (0th), local directory (1st), and file name (2nd). Commonly referred to as UFI.
    static inline UFIMap userFileInfo {};

    static inline std::fstream fileStream {};

    static void openFile(std::string name, bool in);
    static inline void openCloud(bool in) { openFile("scatterSyncCloud.bin", in); }
    static inline void openLocal(bool in) { openFile("scatterSyncLocal.bin", in); }

    static std::string readVariableLen();
    static u_short readIntegral(ByteCount bytes);

    static UFITuple& get(Ident ident);

    static void readCloud();
    static void readLocal();

    static void writeVariableLen(std::string_view value);
    static void writeIntegral(u_long value, ByteCount bytes);

    static void writeCloud();
    static void writeLocal();

public:
    ManifestManip() = delete; // Entirely static class

    // Use the MANI_FOR_EACH(EXEC) macro instead of calling this directly.
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
