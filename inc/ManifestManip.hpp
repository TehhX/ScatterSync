#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <map>
#include <utility>

class ManiManiErr : public std::runtime_error {
public:
    enum ErrCode : u_char {
        FAIL_OPEN,
        FAIL_CLOSE,

        FAIL_READ,
        FAIL_WRITE,

        FAIL_ACCESS,

        FAIL_NAME
    };

    const ErrCode errCode;

    inline ManiManiErr(const std::string& mssg, ErrCode errCode)
    : std::runtime_error { mssg }, errCode { errCode } {}
};

// A class for reading, writing, and accessing static data from the manifest files.
class ManifestManip {
    // The type of userFileInfo, which is a map between unique identifiers and both their generic name and local path.
    using UFI_MAP_TYPE = std::map<u_llong, std::pair<std::string, std::string>>;
    using UFI_MAP_ITER = UFI_MAP_TYPE::iterator;

    static inline const std::string fileNameLocal { "scatterSyncLocal.bin" };
    static inline const std::string fileNameCloud { "scatterSyncCloud.bin" };

    // For each unique identifier, there is a generic name (first) and local path (second).
    static inline UFI_MAP_TYPE userFileInfo {};

    static inline std::fstream fileStream {};

    static void openFile(std::string name);

    static void readCloud();
    static void readLocal();

    static void writeCloud();
    static void writeLocal();

    static void tryAccess(u_llong uniqueIdent);

public:
    ManifestManip() = delete; // Entirely static class

    static inline UFI_MAP_ITER getBegin() { return userFileInfo.begin(); }
    static inline UFI_MAP_ITER getEnd()   { return userFileInfo.end();   }

    static std::string& genNameOf(u_llong uniqueIdent);
    static std::string& localPathOf(u_llong uniqueIdent);
    static std::string  fileNameOf(u_llong uniqueIdent);

    static void readFiles();
    static void writeFiles();

    static void closeFile();
};
