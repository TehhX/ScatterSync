#pragma once

#include <ScatterSyncDefs.hpp>

#include <stdexcept>
#include <string>

class ScatterSyncErr : public std::runtime_error {
public:
    const u_char errCode;

    inline ScatterSyncErr(const std::string& message, u_char errCode)
    : std::runtime_error { message }, errCode { errCode } {}
};
