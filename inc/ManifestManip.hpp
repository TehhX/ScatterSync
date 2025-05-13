#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
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
    enum class ByteCount : u_char {
        CHAR   = 1,
        U_CHAR = CHAR,
        BYTE   = CHAR, // Single byte

        INT    = 2,
        U_INT  = INT,

        LONG   = 4,
        U_LONG = LONG,
        IDENT  = LONG  // Map identifier
    };

    // The type of userFileInfo, which is a map between unique identifiers and both their generic name and local path.
    using ufiPairType = std::pair<std::string, std::string>;
    using ufiMapType = std::map<u_long, ufiPairType>;

    // For each unique identifier, there is a generic name (first) and local path (second).
    static inline ufiMapType userFileInfo {};

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

    static ufiPairType& tryAccess(u_long uniqueIdent);

public:
    ManifestManip() = delete; // Entirely static class

    // TODO: Returning an iterator publically exposes a lot, figure out a more encapsulating way of doing this later
    static inline ufiMapType::iterator getBegin() { return userFileInfo.begin(); }
    static inline ufiMapType::iterator getEnd()   { return userFileInfo.end();   }

    static std::string& genNameOf(u_long uniqueIdent);
    static std::string& localPathOf(u_long uniqueIdent);
    static std::string fileNameOf(u_long uniqueIdent);

    static void readFiles();
    static void writeFiles();

    static void closeFile();
};
