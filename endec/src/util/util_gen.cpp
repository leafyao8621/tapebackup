#include <fcntl.h>
#include <unistd.h>

#include "util.h"

TBENDEC::Util::Gen::Gen() {
    this->fd = open("/dev/urandom", O_RDONLY);
    if (this->fd == -1) {
        throw Err::OPEN;
    }
}

TBENDEC::Util::Gen::~Gen() {
    close(this->fd);
}

void TBENDEC::Util::Gen::operator()(char *buf) const {
    ssize_t bytes_read = read(this->fd, buf, 64);
    if (bytes_read != 64) {
        throw Err::READ;
    }
}
