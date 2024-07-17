#include <iostream>
#include <sstream>
#include <cstring>

#include "app.h"

void TBCLI::App::read(
    char *dev, bool verbose) {
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
    bool failure = false;
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
                char *dev,
                char *key,
                char *md,
                bool verbose,
                std::mutex &mutex,
                bool &failure) {
                try {
                    Util::HMAC hmac(block_size);
                    hmac(dev, key,md, true, verbose, mutex);
                } catch (Util::HMAC::Err) {
                    failure = true;
                }
            },
            this->block_size_hmac,
            dev,
            this->hmac_key,
            this->hmac_md_dev,
            verbose,
            std::ref(this->mutex),
            std::ref(failure)
        );
    this->worker.join();
    if (failure) {
        throw Err::READ;
    }
    if (verbose) {
        Util::get_hex(this->hmac_md_dev, this->buf);
        std::cout << "HMAC on Dev\n" << this->buf << std::endl;
        std::cout << "File valid" << std::endl;
    }
    if (memcmp(this->hmac_md, this->hmac_md_dev, 64)) {
        throw Err::FILE_TAMPERED;
    }
    if (verbose) {
        std::cout << "Read operation completed" << std::endl;
    }
}
