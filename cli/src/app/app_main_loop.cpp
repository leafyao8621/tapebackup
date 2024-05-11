#include <iostream>
#include <unistd.h>
#include "app.h"
#include "../util/util.h"

void TBCLI::App::main_loop() const {
    std::cout << "r/w: ";
    std::cout.flush();
    char res = 0;
    for (std::cin >> res; res != 'r' && res != 'w'; std::cin >> res);
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
    TBCLI::Util::read_archive(this->dev_name);
}

void TBCLI::App::write_dev() const {
    char buf[64];
    TBCLI::Util::check_dev(this->dev_name, buf);
    bool initialized = this->signature.check(buf);
    char res = 0;
    if (!initialized) {
        this->signature.generate(buf);
        std::cout << "Write protect? [y/n]: " ;
        std::cout.flush();
        for (std::cin >> res; res != 'y' && res != 'n'; std::cin >> res);
        TBCLI::Util::init_dev(this->dev_name, buf, res == 'y');
        this->signature.commit(buf);
    }
    bool write_protect =
        TBCLI::Util::check_dev_write_protection(this->dev_name);
    if (initialized && write_protect) {
        throw DEVICE_WRITE_PROTECTED;
    }
    if (initialized) {
        std::cout << "Write protect? [y/n]: " ;
        std::cout.flush();
        for (std::cin >> res; res != 'y' && res != 'n'; std::cin >> res);
        if (res == 'y') {
            TBCLI::Util::set_dev_write_protection(this->dev_name);
        }
    }
    for (;;) {
        std::cout << "Get directory listing? [y/n]: " ;
        std::cout.flush();
        for (std::cin >> res; res != 'y' && res != 'n'; std::cin >> res);
        if (res == 'y') {
            TBCLI::Util::get_dir();
        } else {
            break;
        }
    }
    std::cout << "Directory to backup: " ;
    std::cout.flush();
    std::string dir;
    std::cin >> dir;
    TBCLI::Util::compress_dir((char*)dir.c_str());
    TBCLI::Util::write_archive(this->dev_name);
}
