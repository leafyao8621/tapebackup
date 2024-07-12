#include <cstdlib>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "util.h"

TBCLI::Util::Env::Env() {
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup";
    this->backup = oss.str();
    this->backup_found = false;
    this->backup = oss.str();
}

bool TBCLI::Util::Env::check() {
    struct stat st;
    if (!stat(this->backup.c_str(), &st)) {
        if (S_ISDIR(st.st_mode)) {
            this->backup_found = true;
        }
    }
    return this->backup_found;
}

void TBCLI::Util::Env::init() const {
    if (!this->backup_found) {
        if (mkdir(this->backup.c_str(), 0700)) {
            throw Err::DIRECTORY_CREATION;
        }
    }
}
