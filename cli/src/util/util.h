#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#include <string>

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
            std::string backup, signature;
            bool backup_found, signature_found;
        public:
            enum Err {
                DIRECTORY_CREATION,
                FILE_CREATION
            };
            Env();
            bool check();
            void init() const;
        };
        void check_dev(char *dev_name, char *signature);
        bool check_dev_write_protection(char *dev_name);
        void init_dev(
            char *dev_name, char *signature, bool write_protection);
        void set_dev_write_protection(char *dev_name);
        void get_dir();
        void compress_dir(char *dir);
        void write_archive(char *dev_name);
        void read_archive(char *dev_name);
    }
}

#endif
