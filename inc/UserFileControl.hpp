#pragma once

#include <ScatterSyncDefs.hpp>

#include <ManifestManip.hpp>

#include <string>
#include <stdexcept>
#include <map>

class UserFileErr : public ScatterSyncErr {
public:
    enum ErrCode : u_char {
        DOUBLE_INIT,
        ACTION_ON_MISSING,
        MOVE_ON_UNTRACKED,
        INIT_MISSING,
        INVALID_IDENT
    };

    inline UserFileErr(const std::string& message, ErrCode errCode)
    : ScatterSyncErr { message, errCode } {}
};

class UserFileControl {
public:
    enum class Status : u_char {
        IN_LOCAL,
        IN_REPO,
        MISSING,
        UNTRACKED
    };

    enum class Action : u_char {
        MOVE_TO_REPO,
        MOVE_TO_LOCAL
    };

private:
    static inline bool active { false };

    static inline std::map<ManifestManip::Ident, Status> statusArr {};

    static const Status& searchForAndAssign(ManifestManip::Ident ident);

    static Status& getStatusMutable(ManifestManip::Ident ident);

public:
    UserFileControl() = delete;

    static void init();

    static const Status& getStatus(ManifestManip::Ident ident);

    static void takeAction(ManifestManip::Ident ident, Action action);

    static void takeActionsForEach(Action action);

    static bool exists(std::string_view name);

    static const Status& registerNew(ManifestManip::Ident ident);

    // Returns true if any status is not equal to checkAgainst. Really specific use for GitControl.
    static bool areAnyNotStatus(Status checkAgainst);
};
