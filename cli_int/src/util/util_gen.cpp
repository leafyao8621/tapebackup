#include <fcntl.h>
#include <unistd.h>

#include "util.h"

TBCLI::Util::Gen::Gen() {
    this->fd = open("/dev/urandom", O_RDONLY);
    if (this->fd == -1) {
        throw Err::OPEN;
    }
}

TBCLI::Util::Gen::~Gen() {
    close(this->fd);
}

void TBCLI::Util::Gen::operator()(char *buf) const {
    ssize_t bytes_read = read(this->fd, buf, 64);
    if (bytes_read != 64) {
        throw Err::READ;
    }
}
