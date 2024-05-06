#include <unistd.h>
#include <iostream>
#include "app.h"
#include "../util/util.h"

void TBCLI::App::main_loop() {
    printf("%s: ", "r/w");
    fflush(stdout);
    int res = 0;
    for (res = getchar(); res != 'r' && res != 'w'; res = getchar());
    if (res == 'r') {
        this->read();
    } else {
        this->write();
    }
}

void TBCLI::App::read() {
    bool initialized = TBCLI::Util::check_dev(this->dev);
    if (!initialized) {
        throw Err::DEVICE_NOT_INTIALIZED;
    }
    TBCLI::Util::read_archive(this->dev);
}

void TBCLI::App::write() {
    bool initialized = TBCLI::Util::check_dev(this->dev);
    int res = 0;
    if (!initialized) {
        printf("%s: ", "Write protect? [y/n]");
        fflush(stdout);
        for (res = getchar(); res != 'y' && res != 'n'; res = getchar());
        TBCLI::Util::init_dev(dev, res == 'y');
    }
    bool write_protect = TBCLI::Util::check_dev_write_protection(this->dev);
    if (initialized && write_protect) {
        throw DEVICE_WRITE_PROTECTED;
    }
    if (initialized) {
        printf("%s: ", "Write protect? [y/n]");
        fflush(stdout);
        for (res = getchar(); res != 'y' && res != 'n'; res = getchar());
        if (res == 'y') {
            TBCLI::Util::set_dev_write_protection(this->dev);
        }
    }
    for (;;) {
        printf("%s: ", "Get directory listing? [y/n]");
        fflush(stdout);
        for (res = getchar(); res != 'y' && res != 'n'; res = getchar());
        if (res == 'y') {
            TBCLI::Util::get_dir();
        } else {
            break;
        }
    }
    printf("%s: ", "Directory to backup");
    fflush(stdout);
    std::string dir;
    std::cin >> dir;
    TBCLI::Util::compress_dir((char*)dir.c_str());
    TBCLI::Util::write_archive(this->dev);
}
