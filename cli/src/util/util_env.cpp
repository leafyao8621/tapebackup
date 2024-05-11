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
    std::ostringstream oss_signature;
    oss_signature << getenv("HOME") << "/.signature";
    this->signature = oss_signature.str();
    this->backup_found = false;
    this->signature_found = false;
}

bool TBCLI::Util::Env::check() {
    struct stat st;
    if (!stat(this->backup.c_str(), &st)) {
        if (S_ISDIR(st.st_mode)) {
            this->backup_found = true;
        }
    }
    if (!stat(this->signature.c_str(), &st)) {
        if (!S_ISDIR(st.st_mode)) {
            this->signature_found = true;
        }
    }
    return this->backup_found && this->signature_found;
}

void TBCLI::Util::Env::init() const {
    if (!this->backup_found) {
        if (mkdir(this->backup.c_str(), 0700)) {
            throw Err::DIRECTORY_CREATION;
        }
    }
    if (!this->signature_found) {
        int fd = creat(this->signature.c_str(), 0600);
        if (fd == -1) {
            throw Err::FILE_CREATION;
        }
        close(fd);
    }
}
