#include <sys/stat.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "util.h"

bool TBCLI::Util::check_env() {
    struct stat st;
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup";
    if (stat(oss.str().c_str(), &st)) {
        return false;
    }
    return S_ISDIR(st.st_mode);
}

void TBCLI::Util::init_env() {
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup";
    if (mkdir(oss.str().c_str(), 0700)) {
        throw Err::DIRECTORY_CREATION;
    }
}
