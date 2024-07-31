#include <iostream>
#include <sstream>
#include <cstring>

#include <sys/stat.h>

#include "app.h"

void TBCLI::App::reset(char *dev, char *signature, bool verbose) {
    Util::check_dev(dev, this->signature);
    bool db_check = this->conn.check(this->signature);
    if (!db_check) {
        throw Err::INVALID_SIGNATURE;
    }
    if (memcmp(signature, this->signature, 64)) {
        throw Err::INVALID_SIGNATURE;
    }
    if (verbose) {
        Util::get_hex(this->signature, this->buf);
        std::cout << "Found valid signature\n" << this->buf << std::endl;
    }
    bool write_protect_dev = Util::check_dev_write_protection(dev);
    bool write_protect_db =
        this->conn.get_write_protection(this->signature);
    if (write_protect_db != write_protect_dev) {
        throw Err::WRITE_PROTECT_TAMPERED;
    }
    if (!write_protect_db) {
        if (verbose) {
            std::cout << "Not write protected\n" << std::endl;
        }
        return;
    }
    Util::reset_dev_write_protection(dev);
    this->conn.reset_write_protection(signature);
    if (verbose) {
        std::cout << "Reset operation successful\n" << std::endl;
    }
}
