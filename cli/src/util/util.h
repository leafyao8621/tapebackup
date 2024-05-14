#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#include <string>

#include <openssl/evp.h>

namespace TBCLI {
    namespace Util {
        enum Err {
            DEVICE_OPEN,
            DEVICE_WRITE,
            DIRECTORY_LISTING,
            DIRECTORY_COMPRESSION,
            ARCHIVE_WRITE,
            ARCHIVE_READ,
            READONLY_FLAG
        };
        class Env {
            std::string backup, backup_archive;
            bool backup_found, backup_archive_found;
        public:
            enum Err {
                DIRECTORY_CREATION
            };
            Env();
            bool check();
            void init() const;
        };
        class Gen {
            int fd;
        public:
            enum Err {
                OPEN,
                READ
            };
            Gen();
            ~Gen();
            void operator()(char *buf) const;
        };
        class HMAC {
            EVP_MAC *evp_mac;
            EVP_MAC_CTX *evp_mac_ctx;
            OSSL_PARAM param[2];
        public:
            enum Err {
                CREATION,
                INITIALIZATION,
                UPDATE,
                FINALIZATION
            };
            HMAC();
            ~HMAC();
            void operator()(char *file_name, char *key, char *md) const;
        };
        void check_dev(char *dev_name, char *signature);
        bool check_dev_write_protection(char *dev_name);
        void init_dev(
            char *dev_name, char *signature, bool write_protection);
        void set_dev_write_protection(char *dev_name);
        void get_dir();
        void compress_dir(char *dir);
        void write_archive(char *dev_name, char *file_name);
        void read_archive(char *dev_name, char *file_name);
        void get_hex(char *buf, char *out);
    }
}

#endif
