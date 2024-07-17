#include <iostream>
#include <sstream>
#include <cstring>

#include "app.h"

void TBCLI::App::read(
    char *dev, char *path, bool verbose) {
    if (!this->env.check()) {
        this->env.init();
    }
    Util::check_dev(dev, this->signature);
    bool db_check = this->conn.check(this->signature);
    if (!db_check) {
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
    std::string file_path = this->conn.get_file_name(this->signature);
    size_t slash = file_path.find_last_of('/') + 1;
    std::string file_name = file_path.substr(slash, file_path.size() - slash);
    bool failure = false;
    std::ostringstream oss;
    oss << path << '/' << file_name << ".tar";
    std::string archive_path = oss.str();
    if (verbose) {
        std::cout << "Restoring to " << archive_path << std::endl;
    }
    this->conn.get_key(this->signature, this->hmac_key);
    this->conn.get_hmac(this->signature, this->hmac_md);
    if (verbose) {
        Util::get_hex(this->hmac_md, this->buf);
        std::cout << "HMAC in DB\n" << this->buf << std::endl;
    }
    this->worker =
        std::thread(
            [](
                size_t block_size,
                char *path,
                char *dev,
                bool verbose,
                std::mutex &mutex,
                bool &failure) {
                try {
                    Util::Reader reader(block_size);
                    reader(dev, path, verbose, mutex);
                } catch (Util::Reader::Err) {
                    failure = true;
                }
            },
            this->block_size_read,
            (char*)archive_path.c_str(),
            dev,
            verbose,
            std::ref(this->mutex),
            std::ref(failure)
        );
    this->worker.join();
    if (failure) {
        throw Err::READ;
    }
    Util::HMAC hmac(this->block_size_hmac);
    hmac(
        (char*)archive_path.c_str(),
        this->hmac_key,
        this->hmac_md_dev,
        false,
        verbose,
        this->mutex
    );
    if (verbose) {
        Util::get_hex(this->hmac_md_dev, this->buf);
        std::cout << "HMAC on Dev\n" << this->buf << std::endl;
    }
    if (memcmp(this->hmac_md, this->hmac_md_dev, 64)) {
        throw Err::FILE_TAMPERED;
    }
    if (verbose) {
        std::cout << "Read operation completed" << std::endl;
    }
}
