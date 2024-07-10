#include <fcntl.h>
#include <unistd.h>

#include "util.h"

TBTUI::Util::Writer::Writer(size_t block_size) {
    this->block_size = block_size;
    this->buf = new char[block_size];
    this->fd_in = -1;
    this->fd_out = -1;
}

TBTUI::Util::Writer::~Writer() {
    delete[] this->buf;
    if (this->fd_in != -1) {
        close(this->fd_in);
    }
    if (this->fd_out != -1) {
        close(this->fd_out);
    }
}

void TBTUI::Util::Writer::operator()(
    char *path, char *dev, WINDOW *window, std::mutex &mutex) {
    this->fd_in = open(path, O_RDONLY);
    this->fd_out = open(dev, O_RDWR);
    char buf[128];
    read(this->fd_out, buf, 128);
    ssize_t bytes_read = 0;
    for (
        size_t i = 0;
        (bytes_read = read(this->fd_in, this->buf, this->block_size));
        ++i) {
        if (window) {
            std::lock_guard<std::mutex> guard(mutex);
            wprintw(window, "Writing block %lu\n", i);
            wrefresh(window);
        }
        write(this->fd_out, this->buf, bytes_read);
    }
}
