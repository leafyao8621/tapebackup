#ifndef APP_APP_H_
#define APP_APP_H_

#include <thread>

#include "../db/connector.h"
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
        size_t block_size_hmac, block_size_write, block_size_read;
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
        void write(char *dev, char *path, bool write_protect, bool verbose);
        void read(char *dev, char *path, bool verbose);
        void read(char *dev, bool verbose);
    };
}

#endif
