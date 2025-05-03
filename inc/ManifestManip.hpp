#pragma once

#include <ScatterSyncDefs.hpp>

#include <string>
#include <vector>
#include <fstream>

class ManifestManip {
    struct Cloud {
        u_llong devCount;
        std::vector<std::string> genNames;
        std::vector<u_llong> uniqueIdents;
    };

    struct Local {
        u_llong devIdent;
        std::vector<u_llong> uniqueIdents;
        std::vector<std::string> paths;
    };

    Cloud cloudFile {};
    Local localFile {};

    std::fstream readWrite;

public:
    ManifestManip();

    ~ManifestManip();
};
