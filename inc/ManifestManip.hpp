#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <fstream>
#include <stdexcept>
#include <map>
#include <utility>
#include <functional>

class ManiManiErr : public std::runtime_error {
public:
    enum ErrCode : u_char {
        FAIL_OPEN,
        FAIL_CLOSE,
        FAIL_READ,
        FAIL_WRITE,
        FAIL_ACCESS,
        FAIL_IDENT
    };

    const ErrCode errCode;

    inline ManiManiErr(const std::string& mssg, ErrCode errCode)
    : std::runtime_error { mssg }, errCode { errCode } {}
};

/*
    A class for reading, writing, and accessing static data from the manifest files.
    All integrals are read/written in little endian.
*/
class ManifestManip {
public:
    using UFIPair = std::pair<std::string, std::string>;
    using UFIMap = std::map<u_long, UFIPair>;
    using ForEachFunc = std::function<void(const UFIMap::iterator& iter)>;

private:
    enum class ByteCount : u_char {
        SCROLL_SPEED = 2,
        IDENT        = 4,
        AUTO_SYNC    = 4
    };

    // For each unique identifier, there is a generic name (first) and local path (second). Commonly referred to as UFI.
    static inline UFIMap userFileInfo {};

    static inline std::fstream fileStream {};

    static void openFile(std::string name);
    static inline void openCloud() { openFile("scatterSyncCloud.bin"); }
    static inline void openLocal() { openFile("scatterSyncLocal.bin"); }

    static std::string readVariableLen();
    static u_long readIntegral(ByteCount bytes);

    static void readCloud();
    static void readLocal();

    static void writeVariableLen(std::string_view value);
    static void writeIntegral(u_long value, ByteCount bytes);

    static void writeCloud();
    static void writeLocal();

    static UFIPair& getPair(u_long uniqueIdent);

public:
    ManifestManip() = delete; // Entirely static class

    static void forEach(const ManifestManip::ForEachFunc& forEachFunc);

    static UFIMap::iterator createNewFileMap();

    static std::string& genNameOf(u_long uniqueIdent);
    static std::string& localPathOf(u_long uniqueIdent);
    static std::string fileNameOf(u_long uniqueIdent);

    static void readFiles();
    static void writeFiles();

    static void closeFile();
};
