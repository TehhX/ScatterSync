#include <UserFileControl.hpp>

UserFileErr::UserFileErr(const std::string& message, ErrCode errorCode)
: std::runtime_error { message }, errorCode { errorCode } {}

bool UserFileControl::fileIsInRepo(std::string_view name) {
    // TODO
}

void UserFileControl::moveFile(std::string_view src, std::string_view dest) {
    // TODO
}
