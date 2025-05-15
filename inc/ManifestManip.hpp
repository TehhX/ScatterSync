#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <fstream>
#include <stdexcept>
#include <map>
#include <utility>
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
        FAIL_INDEX
    };

    inline ManiManiErr(const std::string& mssg, ErrCode errCode)
    : ScatterSyncErr { mssg, SC(u_char, errCode) } {}
};

// A class for reading, writing, and accessing static data from the manifest files.
class ManifestManip {
public:
    using Ident       = u_short;
    using GenName     = std::string;
    using LocalPath   = std::string;

    using UFIPair     = std::pair<GenName, LocalPath>;
    using UFIMap      = std::map<Ident, UFIPair>;

private:
    enum class ByteCount : u_char {
        SCROLL_SPEED = 1,
        IDENT        = 2
    };

    static constexpr u_char byteW { 8 }; // Bits/Byte width

    // For each element, there is a generic name (first) and local path (second). Commonly referred to as UFI.
    static inline UFIMap userFileInfo {};

    static inline std::fstream fileStream {};

    static void openFile(std::string name);
    static inline void openCloud() { openFile("scatterSyncCloud.bin"); }
    static inline void openLocal() { openFile("scatterSyncLocal.bin"); }

    static std::string readVariableLen();
    static u_short readIntegral(ByteCount bytes);

    static UFIPair& get(Ident ident);

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

    static std::string& genericNameOf(Ident ident);
    static std::string& localPathOf(Ident ident);
    static std::string fileNameOf(Ident ident);

    static void readFiles();
    static void writeFiles();

    static void closeFile();
};
