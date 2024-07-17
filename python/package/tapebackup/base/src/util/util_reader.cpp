#include <iostream>

#include <fcntl.h>
#include <unistd.h>

#include "util.h"

TBCLI::Util::Reader::Reader(size_t block_size) {
    this->block_size = block_size;
    this->buf = new char[block_size];
    this->fd_in = -1;
    this->fd_out = -1;
}

TBCLI::Util::Reader::~Reader() {
    delete[] this->buf;
    if (this->fd_in != -1) {
        close(this->fd_in);
    }
    if (this->fd_out != -1) {
        close(this->fd_out);
    }
}

void TBCLI::Util::Reader::operator()(
    char *dev, char *path, bool verbose, std::mutex &mutex) {
    this->fd_in = open(dev, O_RDONLY);
    if (fd_in == -1) {
        throw Err::OPEN;
    }
    this->fd_out = open(path, O_WRONLY | O_CREAT, 0600);
    if (fd_out == -1) {
        throw Err::OPEN;
    }
    char buf[64];
    read(this->fd_in, buf, 64);
    read(this->fd_in, buf, 64);
    ssize_t bytes_read = 0;
    for (
        size_t i = 0;
        (bytes_read = read(this->fd_in, this->buf, this->block_size));
        ++i) {
        if (bytes_read == -1) {
            throw Err::READ;
        }
        if (verbose) {
            std::lock_guard<std::mutex> guard(mutex);
            std::cout << "Reading block " << i << std::endl;
        }
        ssize_t bytes_written = write(this->fd_out, this->buf, bytes_read);
        if (bytes_written == -1) {
            throw Err::WRITE;
        }
    }
}
