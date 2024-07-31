#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#include <vector>
#include <string>
#include <mutex>

#include <archive.h>
#include <openssl/evp.h>

namespace TBCLI {
    namespace Util {
        enum Err {
            INVALID_DIR,
            DEVICE_OPEN,
            DEVICE_WRITE,
            DIRECTORY_LISTING,
            DIRECTORY_COMPRESSION,
            ARCHIVE_WRITE,
            ARCHIVE_READ,
            READONLY_FLAG
        };
        class Env {
            std::string backup;
            bool backup_found;
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
        class Archiver {
            struct archive *archive;
            struct archive *disk;
            struct archive_entry *entry;
            bool verbose;
            size_t block_size;
            char *buf;
            int fd;
            struct stat sb;
        public:
            enum Err {
                OPEN,
                APPEND
            };
            Archiver(char *path, size_t block_size, bool verbose);
            ~Archiver();
            void operator()();
        };
        class HMAC {
            EVP_MAC *evp_mac;
            EVP_MAC_CTX *evp_mac_ctx;
            OSSL_PARAM param[2];
            char *buf;
            int fd;
            size_t block_size;
        public:
            enum Err {
                CREATION,
                INITIALIZATION,
                UPDATE,
                FINALIZATION
            };
            HMAC(size_t block_size);
            ~HMAC();
            void operator()(
                char *path,
                char *key,
                char *md,
                bool skip_header,
                bool verbose,
                std::mutex &mutex);
        };
        class Writer {
            int fd_in, fd_out;
            char *buf;
            size_t block_size;
        public:
            enum Err {
                OPEN,
                READ,
                WRITE
            };
            Writer(size_t block_size);
            ~Writer();
            size_t operator()(
                char *path, char *dev, bool verbose, std::mutex &mutex);
        };
        class Reader {
            int fd_in, fd_out;
            char *buf;
            size_t block_size;
        public:
            enum Err {
                OPEN,
                READ,
                WRITE
            };
            Reader(size_t block_size);
            ~Reader();
            void operator()(
                char *dev, char *path, bool verbose, std::mutex &mutex);
        };
        void get_dir(std::string path, std::vector<std::string> &listing);
        void check_dev(char *dev_name, char *signature);
        bool check_dev_write_protection(char *dev_name);
        void init_dev(
            char *dev_name, char *signature, bool write_protection);
        void set_dev_write_protection(char *dev_name);
        void reset_dev_write_protection(char *dev_name);
        void get_hex(char *buf, char *out);
        void parse_hex(char *buf, char *out);
    }
}

#endif
