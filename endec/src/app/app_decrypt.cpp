#include <iostream>
#include <sstream>
#include <cstring>

#include "app.h"

void TBENDEC::App::decrypt(
    char *dev, char *path, bool verbose) {
    if (!this->env.check()) {
        this->env.init();
    }
    std::string dev_path = std::string(dev);
    size_t slash = dev_path.find_last_of('/') + 1;
    std::string dev_name = dev_path.substr(slash, dev_path.size() - slash);
    bool db_check = this->conn.check((char*)dev_name.c_str());
    if (!db_check) {
        throw Err::FILE_TAMPERED;
    }
    this->conn.get_key((char*)dev_name.c_str(), this->hmac_key);
    this->conn.get_hmac((char*)dev_name.c_str(), this->hmac_md);
    if (verbose) {
        Util::get_hex(this->hmac_md, this->buf);
        std::cout << "HMAC in DB\n" << this->buf << std::endl;
    }
    Util::HMAC hmac(this->block_size_hmac);
    hmac(
        dev,
        this->hmac_key,
        this->hmac_md_dev,
        verbose
    );
    if (verbose) {
        Util::get_hex(this->hmac_md, this->buf);
        std::cout << "HMAC on Dev\n" << this->buf << std::endl;
    }
    if (memcmp(this->hmac_md, this->hmac_md_dev, 64)) {
        throw Err::FILE_TAMPERED;
    }
    std::string file_name =
        this->conn.get_file_name((char*)dev_name.c_str());
    std::ostringstream oss;
    oss << path << '/' << file_name << ".tar";
    std::string archive_path = oss.str();
    if (verbose) {
        std::cout << "Restoring to " << archive_path << std::endl;
    }
    {
        Util::Reader reader(this->block_size_read);
        reader(dev, (char*)archive_path.c_str(), this->hmac_key, verbose);
    }
    if (verbose) {
        std::cout << "Read operation completed" << std::endl;
    }
}
