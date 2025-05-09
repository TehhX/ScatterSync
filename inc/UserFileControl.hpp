#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <stdexcept>
#include <filesystem>

namespace fsys = std::filesystem;

class UserFileErr : public std::runtime_error {
public:
    enum ErrCode : u_char {
        FAIL_MOVE,
        FAIL_CHECKSTAT
    };

    const ErrCode errCode;

    inline UserFileErr(const std::string& message, ErrCode errCode)
    : std::runtime_error { message }, errCode { errCode } {}
};

namespace UserFileControl {
    void moveFile(u_llong uniqueIdent, bool inToOut);
};
