#include <unistd.h>
#include <iostream>
#include "app.h"
#include "../util/util.h"

void TBCLI::App::main_loop() const {
    printf("%s: ", "r/w");
    fflush(stdout);
    int res = 0;
    for (res = getchar(); res != 'r' && res != 'w'; res = getchar());
    if (res == 'r') {
        this->read_dev();
    } else {
        this->write_dev();
    }
}

void TBCLI::App::read_dev() const {
    char buf[64];
    TBCLI::Util::check_dev(this->dev_name, buf);
    bool initialized = this->signature.check(buf);
    if (!initialized) {
        throw DEVICE_NOT_INTIALIZED;
    }
    // TBCLI::Util::read_archive(this->dev);
}

void TBCLI::App::write_dev() const {
    char buf[64];
    std::cout << "Checking" << std::endl;
    TBCLI::Util::check_dev(this->dev_name, buf);
    bool initialized = this->signature.check(buf);
    int res = 0;
    if (!initialized) {
        this->signature.generate(buf);
        printf("%s: ", "Write protect? [y/n]");
        fflush(stdout);
        for (res = getchar(); res != 'y' && res != 'n'; res = getchar());
        TBCLI::Util::init_dev(this->dev_name, buf, res == 'y');
        this->signature.commit(buf);
    }
    bool write_protect =
        TBCLI::Util::check_dev_write_protection(this->dev_name);
    if (initialized && write_protect) {
        throw DEVICE_WRITE_PROTECTED;
    }
    if (initialized) {
        printf("%s: ", "Write protect? [y/n]");
        fflush(stdout);
        for (res = getchar(); res != 'y' && res != 'n'; res = getchar());
        if (res == 'y') {
            TBCLI::Util::set_dev_write_protection(this->dev_name);
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
    // TBCLI::Util::write_archive(this->dev);
}
