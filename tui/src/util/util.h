#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#include <vector>
#include <string>

namespace TBTUI {
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
        void get_dir(std::string path, std::vector<std::string> &listing);
        void check_dev(char *dev_name, char *signature);
        bool check_dev_write_protection(char *dev_name);
        void init_dev(
            char *dev_name, char *signature, bool write_protection);
        void set_dev_write_protection(char *dev_name);
        void get_hex(char *buf, char *out);
    }
}

#endif
