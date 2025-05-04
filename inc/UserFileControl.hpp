#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <stdexcept>

class UserFileErr : public std::runtime_error {
public:
    enum ErrCode : u_char {
        FAIL_MOVE
    };

    const ErrCode errorCode;

    UserFileErr(const std::string& message, ErrCode errorCode);
};

class UserFileControl {
public:
    UserFileControl() = delete;

    static bool fileIsInRepo(std::string_view name);
    static void moveFile(std::string_view src, std::string_view dest);
};
