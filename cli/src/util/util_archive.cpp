#include <unistd.h>
#include <sstream>
#include "util.h"

void TBCLI::Util::write_archive(FILE *dev) {
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup/backup.tar.gz";
    fseek(dev, 4, 0);
    FILE *fin = fopen(oss.str().c_str(), "rb");
    if (!fin) {
        throw Err::ARCHIVE_WRITE;
    }
    char buf[512];
    size_t read = 0, written = 0;
    for (; (read = fread(buf, 1, 512, fin));) {
        written = fwrite(buf, 1, read, dev);
        fflush(dev);
        if (read != written) {
            throw Err::ARCHIVE_WRITE;
        }
    }
    fclose(fin);
    unsigned trailer = 0xdeadbeef;
    written = fwrite(&trailer, 4, 1, dev);
    if (written != 1) {
        throw Err::ARCHIVE_WRITE;
    }
}

void TBCLI::Util::read_archive(FILE *dev) {
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup/restore.tar.gz";
    fseek(dev, 4, 0);
    truncate(oss.str().c_str(), 0);
    FILE *fout = fopen(oss.str().c_str(), "wb");
    if (!fout) {
        throw Err::ARCHIVE_WRITE;
    }
    char buf[512];
    size_t read = 0, written = 0;
    for (; (read = fread(buf, 1, 512, dev));) {
        char *iter = buf;
        size_t offset = 0;
        for (offset = 0; offset < read; ++offset, ++iter) {
            if (offset > 508) {
                continue;
            }
            if (*(unsigned*)iter == 0xdeadbeef) {
                break;
            }
        }
        written = fwrite(buf, 1, offset, fout);
        if (written != offset) {
            throw Err::ARCHIVE_READ;
        }
        if (written < 512) {
            break;
        }
    }
    fclose(fout);
}
