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
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    read(dev, &buf, 128);
    close(dev);
    std::ostringstream oss_dd;
    oss_dd <<
        "dd if=" <<
        getenv("HOME") << "/backup/backup.tar.gz" <<
        " of=" << dev_name <<
        " status=progress";
    system(oss_dd.str().c_str());
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
