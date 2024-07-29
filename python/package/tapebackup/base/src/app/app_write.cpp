#include <iostream>
#include <sstream>

#include <sys/stat.h>

#include "app.h"

void TBCLI::App::write(
    char *dev, char *path, bool write_protect, bool verbose) {
    if (!this->env.check()) {
        this->env.init();
    }
    Util::check_dev(dev, this->signature);
    bool db_check = this->conn.check(this->signature);
    if (db_check) {
        if (verbose) {
            Util::get_hex(this->signature, this->buf);
            std::cout << "Found valid signature\n" << this->buf << std::endl;
        }
        bool write_protect_dev = Util::check_dev_write_protection(dev);
        bool write_protect_db =
            this->conn.get_write_protection(this->signature);
        if (write_protect_db != write_protect_dev) {
            throw Err::WRITE_PROTECT_TAMPERED;
        } else if (write_protect_dev) {
            throw Err::WRITE_PROTECTED;
        }
        if (write_protect) {
            Util::set_dev_write_protection(dev);
            this->conn.set_write_protection(this->signature);
        }
    } else {
        if (verbose) {
            std::cout << "Creating new" << std::endl;
        }
        this->gen(this->signature);
        if (verbose) {
            Util::get_hex(this->signature, this->buf);
            std::cout << "Generated signature\n" << this->buf << std::endl;
        }
        Util::init_dev(dev, this->signature, write_protect);
        this->conn.add(this->signature, write_protect);
    }
    time_t start_time = time(NULL);
    this->conn.set_start_time(this->signature, start_time);
    this->conn.update_file_name(this->signature, path);
    {
        Util::Archiver archiver(path, this->block_size_archive, verbose);
        archiver();
    }
    bool failure = false;
    size_t written_size = 0;
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup/backup.tar";
    std::string archive_path = oss.str();
    struct stat st;
    stat(archive_path.c_str(), &st);
    this->conn.set_reported_size(this->signature, st.st_size);
    if (verbose) {
        std::cout << "Reported size " << st.st_size << std::endl;
    }
    this->gen(this->hmac_key);
    if (verbose) {
        Util::get_hex(this->hmac_key, this->buf);
        std::cout << "Generated HMAC Key\n" << this->buf << std::endl;
    }
    this->conn.update_key(this->signature, this->hmac_key);
    this->worker =
        std::thread(
            [](
                size_t block_size,
                char *path,
                char *dev,
                bool verbose,
                std::mutex &mutex,
                bool &failure,
                size_t &written_size) {
                try {
                    Util::Writer writer(block_size);
                    written_size = writer(path, dev, verbose, mutex);
                } catch (Util::Writer::Err) {
                    failure = true;
                }
            },
            this->block_size_write,
            (char*)archive_path.c_str(),
            dev,
            verbose,
            std::ref(this->mutex),
            std::ref(failure),
            std::ref(written_size)
        );
    Util::HMAC hmac(this->block_size_hmac);
    hmac(
        (char*)archive_path.c_str(),
        this->hmac_key,
        this->hmac_md,
        false,
        verbose,
        this->mutex
    );
    this->worker.join();
    if (failure) {
        throw Err::WRITE;
    }
    this->conn.update_hmac(this->signature, this->hmac_md);
    if (verbose) {
        Util::get_hex(this->hmac_md, this->buf);
        std::cout << "HMAC Value " << this->buf << std::endl;
    }
    this->conn.set_written_size(this->signature, written_size);
    if (verbose) {
        std::cout << "Written size " << written_size << std::endl;
    }
    if (verbose) {
        std::cout << "Write operation completed" << std::endl;
    }
    time_t completion_time = time(NULL);
    this->conn.set_completion_time(this->signature, completion_time);
    this->conn.add_transaction(
        this->signature,
        write_protect,
        this->hmac_key,
        path,
        this->hmac_md,
        start_time,
        completion_time,
        st.st_size,
        written_size
    );
}
