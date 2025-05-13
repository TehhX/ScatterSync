#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <utility>

class ManiManiErr : public std::runtime_error {
public:
    enum ErrCode : u_char {
        FAIL_OPEN,
        FAIL_CLOSE,
        FAIL_READ,
        FAIL_WRITE,
        FAIL_ACCESS,
        FAIL_INDEX
    };

    const ErrCode errCode;

    inline ManiManiErr(const std::string& mssg, ErrCode errCode)
    : std::runtime_error { mssg }, errCode { errCode } {}
};

// A class for reading, writing, and accessing static data from the manifest files.
class ManifestManip {
public:
    using UFIPair = std::pair<std::string, std::string>;
    using UFIVector = std::vector<UFIPair>;

private:
    enum class ByteCount : u_char {
        SCROLL_SPEED = 2,
        AUTO_SYNC    = 4
    };

    // For each element, there is a generic name (first) and local path (second). Commonly referred to as UFI.
    static inline UFIVector userFileInfo {};

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

public:
    ManifestManip() = delete; // Entirely static class

    // Returns index of new file element
    static size_t createNewFileElement();

    static std::string& genericNameOf(size_t index);
    static std::string& localPathOf(size_t index);
    static std::string fileNameOf(size_t index);

    static inline size_t size() { return userFileInfo.size(); }

    static void readFiles();
    static void writeFiles();

    static void closeFile();
};
