#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "util.h"

void TBCLI::Util::check_dev(char *dev_name, char *signature) {
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    read(dev, signature, 64);
    close(dev);
}

bool TBCLI::Util::check_dev_write_protection(char *dev_name) {
    char buf[64];
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    read(dev, &buf, 64);
    read(dev, &buf, 64);
    close(dev);
    char all_one[64];
    char all_zero[64] = {0};
    memset(all_one, 0xff, 64);
    if (!memcmp(all_one, buf, 64)) {
        return true;
    }
    if (!memcmp(all_zero, buf, 64)) {
        return false;
    }
    throw TBCLI::Util::Err::READONLY_FLAG;
}

void TBCLI::Util::init_dev(
    char *dev_name, char *signature, bool write_protection) {
    int dev = open(dev_name, O_WRONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    ssize_t bytes_written = write(dev, signature, 64);
    if (bytes_written != 64) {
        close(dev);
        throw DEVICE_WRITE;
    }
    char write_protection_value[64];
    memset(write_protection_value, write_protection ? 0xff : 0, 64);
    bytes_written = write(dev, write_protection_value, 64);
    close(dev);
    if (bytes_written != 64) {
        throw DEVICE_WRITE;
    }
}

void TBCLI::Util::set_dev_write_protection(char *dev_name) {
    char buf[64];
    int dev = open(dev_name, O_RDWR);
    if (dev == -1) {
        close(dev);
        throw DEVICE_OPEN;
    }
    read(dev, buf, 64);
    memset(buf, 0xff, 64);
    write(dev, buf, 64);
    close(dev);
}
