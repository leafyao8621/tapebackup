#include <iostream>
#include <sstream>
#include <fcntl.h>

#include <unistd.h>

#include "util.h"

void TBCLI::Util::check_dev(char *dev_name, char *signature) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    read(dev, signature, 64);
    system(oss.str().c_str());
}

bool TBCLI::Util::check_dev_write_protection(char *dev_name) {
    unsigned short signature = 0;
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    read(dev, &signature, 2);
    system(oss.str().c_str());
    return signature == 0x90b0;
}

void TBCLI::Util::init_dev(
    char *dev_name, char *signature, bool write_protection) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    ssize_t bytes_written = write(dev, signature, 64);
    if (bytes_written != 64) {
        throw DEVICE_WRITE;
    }
    unsigned short write_protection_value = write_protection ? 0x90b0 : 0xb090;
    bytes_written = write(dev, &write_protection_value, 2);
    if (bytes_written != 2) {
        throw DEVICE_WRITE;
    }
    system(oss.str().c_str());
}

void TBCLI::Util::set_dev_write_protection(char *dev_name) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    unsigned short signature = 0x90b0;
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    write(dev, &signature, 2);
    system(oss.str().c_str());
}
