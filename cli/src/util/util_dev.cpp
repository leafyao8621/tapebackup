#include <unistd.h>

#include "util.h"

void TBCLI::Util::check_dev(int dev, char *signature) {
    read(dev, signature, 64);
    lseek(dev, -64, SEEK_CUR);
}

bool TBCLI::Util::check_dev_write_protection(int dev) {
    lseek(dev, 64, SEEK_CUR);
    unsigned short signature = 0;
    read(dev, &signature, 2);
    lseek(dev, -64, SEEK_CUR);
    return signature == 0x90b0;
}

void TBCLI::Util::init_dev(int dev, char *signature, bool write_protection) {
    ssize_t bytes_written = write(dev, signature, 64);
    if (bytes_written != 64) {
        throw DEVICE_WRITE;
    }
    unsigned short write_protection_value = write_protection ? 0x90b0 : 0xb090;
    bytes_written = write(dev, &write_protection_value, 2);
    if (bytes_written != 2) {
        throw DEVICE_WRITE;
    }
}

void TBCLI::Util::set_dev_write_protection(int dev) {
    lseek(dev, 64, SEEK_CUR);
    unsigned short signature = 0x90b0;
    write(dev, &signature, 2);
    lseek(dev, -64, SEEK_CUR);
}
