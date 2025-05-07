#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

class ManiManiErr : public std::runtime_error {
public:
    enum ErrCode : u_char {
        FAIL_OPEN,
        FAIL_CLOSE,

        FAIL_READ,
        FAIL_WRITE
    };

    const ErrCode errCode;

    inline ManiManiErr(const std::string& mssg, ErrCode errCode)
    : std::runtime_error { mssg }, errCode { errCode } {}
};

// A class for reading, writing, and accessing static data from the manifest files.
class ManifestManip {
    struct Cloud {
        u_llong devCount;
        std::vector<u_llong> uniqueIdents;
        std::vector<std::string> genNames;
    };

    struct Local {
        std::vector<u_llong> uniqueIdents;
        std::vector<std::string> paths;
    };

    static inline constexpr char* fileNameLocal { "scatterSyncLocal.bin" };
    static inline constexpr char* fileNameCloud { "scatterSyncCloud.bin" };

    static inline std::fstream fileStream;

    static void openFile(const char* name);

    static void readCloud();
    static void readLocal();

    static void writeCloud();
    static void writeLocal();

public:
    static Cloud cloudFile;
    static Local localFile;

    ManifestManip() = delete; // Entirely static class

    static void readFiles();
    static void writeFiles();

    static void closeFile();
};
