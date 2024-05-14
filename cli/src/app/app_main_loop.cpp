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
    bool initialized = this->connector.check(buf);
    if (!initialized) {
        throw DEVICE_NOT_INTIALIZED;
    }
    bool write_protect =
        TBCLI::Util::check_dev_write_protection(this->dev_name);
    bool write_protect_db =
        this->connector.get_write_protection(buf);
    if (write_protect != write_protect_db) {
        throw DEVICE_WRITE_PROTECTION_TAMPERED;
    }
    // TBCLI::Util::read_archive(this->dev_name);
}

void TBCLI::App::write_dev() const {
    char buf[64];
    TBCLI::Util::check_dev(this->dev_name, buf);
    bool initialized = this->connector.check(buf);
    char res = 0;
    if (!initialized) {
        this->gen(buf);
        std::cout << "Write protect? [y/n]: " ;
        std::cout.flush();
        for (std::cin >> res; res != 'y' && res != 'n'; std::cin >> res);
        TBCLI::Util::init_dev(this->dev_name, buf, res == 'y');
        this->connector.add(buf, res == 'y');
    }
    bool write_protect =
        TBCLI::Util::check_dev_write_protection(this->dev_name);
    bool write_protect_db =
        this->connector.get_write_protection(buf);
    if (write_protect != write_protect_db) {
        throw DEVICE_WRITE_PROTECTION_TAMPERED;
    }
    if (initialized && write_protect) {
        throw DEVICE_WRITE_PROTECTED;
    }
    if (initialized) {
        std::cout << "Write protect? [y/n]: " ;
        std::cout.flush();
        for (std::cin >> res; res != 'y' && res != 'n'; std::cin >> res);
        if (res == 'y') {
            TBCLI::Util::set_dev_write_protection(this->dev_name);
            this->connector.set_write_protection(buf);
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
    // TBCLI::Util::write_archive(this->dev_name);
}
