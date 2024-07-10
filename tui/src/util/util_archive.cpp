#include <iostream>
#include <sstream>
#include <fcntl.h>

#include "util.h"

TBTUI::Util::Archiver::Archiver() {
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup/backup.tar";
    remove(oss.str().c_str());
    tar_open(
        &this->tar,
        oss.str().c_str(),
        NULL,
        O_WRONLY | O_CREAT,
        0600,
        TAR_IGNORE_MAGIC
    );
}

TBTUI::Util::Archiver::~Archiver() {
    tar_close(this->tar);
}

void TBTUI::Util::Archiver::operator()(char *path) {
    tar_append_tree(this->tar, path, (char*)".");
    tar_append_eof(this->tar);
}
