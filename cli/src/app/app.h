#ifndef APP_APP_H_
#define APP_APP_H_

#include <thread>

#include "../db/connector.h"
#include "../report/report.h"
#include "../util/util.h"

namespace TBCLI {
    class App {
        Connector conn;
        Util::Gen gen;
        Util::Env env;
        std::thread worker;
        char signature[64];
        char hmac_key[64];
        char hmac_md[64];
        char hmac_md_dev[64];
        char buf[129];
        size_t block_size_hmac;
        size_t block_size_write;
        size_t block_size_read;
        size_t block_size_archive;
        std::mutex mutex;
    public:
        enum Err {
            WRITE_PROTECT_TAMPERED,
            WRITE_PROTECTED,
            WRITE,
            INVALID_SIGNATURE,
            READ,
            FILE_TAMPERED
        };
        App();
        void set_block_size_hmac(size_t size);
        void set_block_size_write(size_t size);
        void set_block_size_read(size_t size);
        void set_block_size_archive(size_t size);
        void write(char *dev, char *path, bool write_protect, bool verbose);
        void read(char *dev, char *path, bool verbose);
        void reset(char *dev, char *signature, bool verbose);
        void report_daily(
            std::string beginning,
            std::string ending,
            Report::Format format,
            std::ostream &os);
        void report_list(
            std::string beginning,
            std::string ending,
            Report::Format format,
            std::ostream &os);
        void report_lookup(
            std::string path,
            Report::Format format,
            std::ostream &os);
        void report_transaction(
            std::string beginning,
            std::string ending,
            Report::Format format,
            std::ostream &os);
    };
}

#endif
