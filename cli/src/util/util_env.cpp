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
    std::ostringstream oss_archive;
    oss_archive << getenv("HOME") << "/backup/archive";
    this->backup = oss.str();
    this->backup_archive = oss_archive.str();
    this->backup_found = false;
    this->backup_archive_found = false;
}

bool TBCLI::Util::Env::check() {
    struct stat st;
    if (!stat(this->backup.c_str(), &st)) {
        if (S_ISDIR(st.st_mode)) {
            this->backup_found = true;
        }
    }
    if (!stat(this->backup_archive.c_str(), &st)) {
        if (S_ISDIR(st.st_mode)) {
            this->backup_archive_found = true;
        }
    }
    return this->backup_found && this->backup_archive_found;
}

void TBCLI::Util::Env::init() const {
    if (!this->backup_found) {
        if (mkdir(this->backup.c_str(), 0700)) {
            throw Err::DIRECTORY_CREATION;
        }
    }
    if (!this->backup_archive_found) {
        if (mkdir(this->backup_archive.c_str(), 0700)) {
            throw Err::DIRECTORY_CREATION;
        }
    }
}
