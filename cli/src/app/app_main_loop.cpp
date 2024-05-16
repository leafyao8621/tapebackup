#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
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
    std::string file_name = this->connector.get_file_name(buf);
    TBCLI::Util::read_archive(this->dev_name, (char*)file_name.c_str());
    char key[64], hmac[64], hmac_read[64];
    this->connector.get_key(buf, key);
    this->connector.get_hmac(buf, hmac);
    this->hmac((char*)file_name.c_str(), key, hmac_read);
    if (memcmp(hmac, hmac_read, 64)) {
        throw Err::DEVICE_FILE_TAMPERED;
    }
}

void TBCLI::App::write_dev() const {
    char signature[64], key[64];
    TBCLI::Util::check_dev(this->dev_name, signature);
    bool initialized = this->connector.check(signature);
    char res = 0;
    if (!initialized) {
        this->gen(signature);
        std::cout << "Write protect? [y/n]: " ;
        std::cout.flush();
        for (std::cin >> res; res != 'y' && res != 'n'; std::cin >> res);
        TBCLI::Util::init_dev(this->dev_name, signature, res == 'y');
        this->connector.add(signature, res == 'y');
    }
    bool write_protect =
        TBCLI::Util::check_dev_write_protection(this->dev_name);
    bool write_protect_db =
        this->connector.get_write_protection(signature);
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
            this->connector.set_write_protection(signature);
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
    this->connector.update_file_name(signature, (char*)dir.c_str());
    TBCLI::Util::compress_dir((char*)dir.c_str());
    this->gen(key);
    this->connector.update_key(signature, key);
    char hmac[64];
    this->hmac((char*)dir.c_str(), key, hmac);
    this->connector.update_hmac(signature, hmac);
    std::cout << "Get information file? [y/n]: " ;
    char signature_out[129], key_out[129], hmac_out[129];
    TBCLI::Util::get_hex(signature, signature_out);
    TBCLI::Util::get_hex(key, key_out);
    TBCLI::Util::get_hex(hmac, hmac_out);
    std::cout << "File name: " << dir << std::endl <<
        "Signature: " << signature_out << std::endl <<
        "Key: " << key_out << std::endl <<
        "HMAC: " << hmac_out << std::endl;
    {
        std::ostringstream oss_ofn;
        oss_ofn << dir << ".txt";
        std::ofstream ofs(oss_ofn.str());
        ofs << "File name: " << dir << std::endl <<
            "Signature: " << signature_out << std::endl <<
            "Key: " << key_out << std::endl <<
            "HMAC: " << hmac_out << std::endl;
    }
    TBCLI::Util::write_archive(this->dev_name, (char*)dir.c_str());
}
