#include <iostream>
#include <sstream>
#include <fcntl.h>

#include <unistd.h>

#include "util.h"

void TBCLI::Util::check_dev(char *dev_name, char *signature) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    char res = 'n';
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res == 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    read(dev, signature, 64);
    close(dev);
    system(oss.str().c_str());
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res == 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
    getchar();
}

bool TBCLI::Util::check_dev_write_protection(char *dev_name) {
    unsigned short signature = 0;
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    char res = 'n';
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res == 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    read(dev, &signature, 2);
    close(dev);
    system(oss.str().c_str());
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res == 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
    return signature == 0x90b0;
}

void TBCLI::Util::init_dev(
    char *dev_name, char *signature, bool write_protection) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    char res = 'n';
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res == 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        throw DEVICE_OPEN;
    }
    ssize_t bytes_written = write(dev, signature, 64);
    if (bytes_written != 64) {
        close(dev);
        throw DEVICE_WRITE;
    }
    unsigned short write_protection_value = write_protection ? 0x90b0 : 0xb090;
    bytes_written = write(dev, &write_protection_value, 2);
    close(dev);
    if (bytes_written != 2) {
        throw DEVICE_WRITE;
    }
    system(oss.str().c_str());
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res == 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
}

void TBCLI::Util::set_dev_write_protection(char *dev_name) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    char res = 'n';
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res == 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
    unsigned short signature = 0x90b0;
    int dev = open(dev_name, O_RDONLY);
    if (dev == -1) {
        close(dev);
        throw DEVICE_OPEN;
    }
    write(dev, &signature, 2);
    close(dev);
    system(oss.str().c_str());
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res == 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
}
