#include <iostream>
#include <sstream>
#include <fcntl.h>

#include "util.h"

TBCLI::Util::Archiver::Archiver() {
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup/backup.tar";
    remove(oss.str().c_str());
    int ret =
        tar_open(
            &this->tar,
            oss.str().c_str(),
            NULL,
            O_WRONLY | O_CREAT,
            0600,
            TAR_IGNORE_MAGIC
        );
    if (ret) {
        throw Err::OPEN;
    }
}

TBCLI::Util::Archiver::~Archiver() {
    tar_close(this->tar);
}

void TBCLI::Util::Archiver::operator()(char *path) {
    int ret =
        tar_append_tree(this->tar, path, (char*)".");
    if (ret) {
        throw Err::APPEND;
    }
    ret = tar_append_eof(this->tar);
    if (ret) {
        throw Err::APPEND;
    }
}
