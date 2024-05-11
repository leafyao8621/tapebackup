#include <iostream>
#include <sstream>

#include <unistd.h>

#include "util.h"

void TBCLI::Util::check_dev(int dev, char *signature, char *dev_name) {
    read(dev, signature, 64);
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
}

bool TBCLI::Util::check_dev_write_protection(int dev, char *dev_name) {
    lseek(dev, 64, SEEK_CUR);
    unsigned short signature = 0;
    read(dev, &signature, 2);
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    return signature == 0x90b0;
}

void TBCLI::Util::init_dev(
    int dev, char *signature, bool write_protection, char *dev_name) {
    ssize_t bytes_written = write(dev, signature, 64);
    if (bytes_written != 64) {
        throw DEVICE_WRITE;
    }
    unsigned short write_protection_value = write_protection ? 0x90b0 : 0xb090;
    bytes_written = write(dev, &write_protection_value, 2);
    if (bytes_written != 2) {
        throw DEVICE_WRITE;
    }
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
}

void TBCLI::Util::set_dev_write_protection(int dev, char *dev_name) {
    lseek(dev, 64, SEEK_CUR);
    unsigned short signature = 0x90b0;
    write(dev, &signature, 2);
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
}
