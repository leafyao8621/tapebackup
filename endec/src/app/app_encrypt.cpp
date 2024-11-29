#include <iostream>
#include <sstream>

#include <sys/stat.h>

#include "app.h"

void TBENDEC::App::encrypt(
    char *dev, char *path, bool verbose) {
    if (!this->env.check()) {
        this->env.init();
    }
    {
        Util::Archiver archiver(path, this->block_size_archive, verbose);
        archiver();
    }
    bool failure = false;
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup/endec.tar";
    std::string archive_path = oss.str();
    std::string file_path = std::string(path);
    size_t slash = file_path.find_last_of('/') + 1;
    std::string file_name = file_path.substr(slash, file_path.size() - slash);
    this->gen(this->hmac_key);
    if (verbose) {
        Util::get_hex(this->hmac_key, this->buf);
        std::cout << "Generated HMAC Key\n" << this->buf << std::endl;
    }
    {
        Util::Writer writer(this->block_size_write);
        writer((char*)oss.str().c_str(), dev, this->hmac_key, verbose);
    }
    Util::HMAC hmac(this->block_size_hmac);
    hmac(
        dev,
        this->hmac_key,
        this->hmac_md,
        verbose
    );
    if (failure) {
        throw Err::WRITE;
    }
    if (verbose) {
        Util::get_hex(this->hmac_md, this->buf);
        std::cout << "HMAC Value " << this->buf << std::endl;
    }
    std::string dev_path = std::string(dev);
    slash = dev_path.find_last_of('/') + 1;
    std::string dev_name = dev_path.substr(slash, dev_path.size() - slash);
    this->conn.add(
        (char*)dev_name.c_str(),
        this->hmac_md,
        this->hmac_key,
        (char*)file_name.c_str());
    if (verbose) {
        std::cout << "Encrypt operation completed" << std::endl;
    }
}
