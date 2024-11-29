#ifndef APP_APP_H_
#define APP_APP_H_

#include "../db/connector.h"
#include "../util/util.h"

namespace TBENDEC {
    class App {
        Connector conn;
        Util::Gen gen;
        Util::Env env;
        char hmac_key[64];
        char hmac_md[64];
        char hmac_md_dev[64];
        char buf[129];
        size_t block_size_hmac;
        size_t block_size_write;
        size_t block_size_read;
        size_t block_size_archive;
    public:
        enum Err {
            FILE_TAMPERED,
            WRITE,
            READ
        };
        App();
        void set_block_size_hmac(size_t size);
        void set_block_size_write(size_t size);
        void set_block_size_read(size_t size);
        void set_block_size_archive(size_t size);
        void encrypt(char *dev, char *path, bool verbose);
        void decrypt(char *dev, char *path, bool verbose);
    };
}

#endif
