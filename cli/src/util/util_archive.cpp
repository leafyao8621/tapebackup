#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include "util.h"

void TBCLI::Util::write_archive(char *dev_name) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    char buf[128];
    int dev = open(dev_name, O_RDWR);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    read(dev, &buf, 128);
    std::ostringstream oss_dd;
    oss_dd << getenv("HOME") << "/backup/backup.tar.gz";
    int archive = open(oss_dd.str().c_str(), O_RDONLY);
    char wr_buf[1024];
    int bytes_read = 0;
    for (; (bytes_read = read(archive, wr_buf, 1024)) == 1024; ) {
        write(dev, wr_buf, 1024);
    }
    if (bytes_read > 0) {
        write(dev, wr_buf, bytes_read);
    }
    close(archive);
    close(dev);
}

void TBCLI::Util::read_archive(char *dev_name) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    char buf[128];
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    read(dev, &buf, 128);
    close(dev);
    std::ostringstream oss_dd;
    oss_dd <<
        "dd if=" <<
        dev_name <<
        " of=" <<
        getenv("HOME") << "/backup/restore.tar.gz" <<
        " status=progress";
    system(oss_dd.str().c_str());
}
