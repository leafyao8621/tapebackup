#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#include <string>

namespace TBCLI {
    namespace Util {
        enum Err {
            DEVICE_WRITE,
            DIRECTORY_LISTING,
            DIRECTORY_COMPRESSION,
            ARCHIVE_WRITE,
            ARCHIVE_READ
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
        void check_dev(int dev, char *signature, char *dev_name);
        bool check_dev_write_protection(int dev, char *dev_name);
        void init_dev(
            int dev, char *signature, bool write_protection, char *dev_name);
        void set_dev_write_protection(int dev, char *dev_names);
        void get_dir();
        void compress_dir(char *dir);
        void write_archive(int dev);
        void read_archive(int dev);
    }
}

#endif
