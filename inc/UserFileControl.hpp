#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <stdexcept>

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
    bool fileIsInRepo(std::string_view name);
    void moveFile(std::string_view src, std::string_view dest);
}
