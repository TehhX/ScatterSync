#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <stdexcept>
#include <vector>

class UserFileErr : public ScatterSyncErr {
public:
    enum ErrCode : u_char {
        DOUBLE_INIT,
        ACTION_ON_MISSING,
        INIT_MISSING
    };

    inline UserFileErr(const std::string& message, ErrCode errCode)
    : ScatterSyncErr { message, errCode } {}
};

class UserFileControl {
public:
    enum class Status : u_char {
        IN_LOCAL,
        IN_REPO,
        MISSING
    };

    enum class Action : u_char {
        MOVE_TO_REPO,
        MOVE_TO_LOCAL,
        REMOVE
    };

private:
    static inline bool active { false };

    static inline std::vector<Status> statusArr {};

    static const Status& searchForAndAssign(size_t index);

public:
    UserFileControl() = delete;

    static void init();

    static inline const Status& getStatus(size_t index) { return statusArr[index]; }
    static void takeAction(size_t index, Action action);

    static void takeActionsAll(Action action);

    static bool exists(std::string_view name);

    static const Status& registerNew(size_t index);

    static inline size_t size() { return statusArr.size(); }
};
