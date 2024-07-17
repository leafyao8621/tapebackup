#include <iostream>
#include <sstream>

#include <fcntl.h>

#include <archive_entry.h>

#include "util.h"

TBCLI::Util::Archiver::Archiver(char *path, size_t block_size, bool verbose) {
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup/backup.tar";
    remove(oss.str().c_str());
    this->block_size = block_size;
    this->verbose = verbose;
    this->buf = new char[block_size];
    this->archive = NULL;
    this->disk = NULL;
    this->entry = NULL;
    this->archive = archive_write_new();
    archive_write_add_filter_none(this->archive);
    archive_write_set_format_gnutar(this->archive);
    if (archive_write_open_filename(this->archive, oss.str().c_str())) {
        throw Err::OPEN;
    }
    this->disk = archive_read_disk_new();
    if (archive_read_disk_open(this->disk, path)) {
        throw Err::OPEN;
    }
    archive_read_disk_set_standard_lookup(this->disk);
    this->entry = archive_entry_new();
    this->fd = -1;
}

TBCLI::Util::Archiver::~Archiver() {
    delete[] this->buf;
    if (this->fd != -1) {
        close(this->fd);
    }
    if (this->entry) {
        archive_entry_free(this->entry);
    }
    if (this->disk) {
        archive_read_close(this->disk);
        archive_read_free(this->disk);
    }
    if (this->archive) {
        archive_write_close(this->archive);
        archive_write_free(this->archive);
    }
}

void TBCLI::Util::Archiver::operator()() {
    for (;;) {
        archive_entry_clear(this->entry);
        int ret = archive_read_next_header2(this->disk, this->entry);
        if (ret == ARCHIVE_EOF) {
            break;
        }
        if (ret) {
            throw Err::APPEND;
        }
        archive_read_disk_descend(this->disk);
        if (this->verbose) {
            std::cout << archive_entry_pathname(this->entry) << std::endl;
        }
        ret = archive_write_header(this->archive, this->entry);
        if (ret) {
            throw Err::APPEND;
        }
        stat(archive_entry_sourcepath(entry), &this->sb);
        if (S_ISDIR(this->sb.st_mode)) {
            continue;
        }
        this->fd = open(archive_entry_sourcepath(entry), O_RDONLY);
        if (this->fd == -1) {
            throw Err::APPEND;
        }
        ssize_t bytes_read = 0;
        la_ssize_t bytes_written = 0;
        for (; (bytes_read = read(fd, this->buf, this->block_size));) {
            if (bytes_read == -1) {
                throw Err::APPEND;
            }
            bytes_written =
                archive_write_data(this->archive, this->buf, bytes_read);
            if (bytes_written == -1) {
                throw Err::APPEND;
            }
        }
        close(this->fd);
        this->fd = -1;
    }
}
