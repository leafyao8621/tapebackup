#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#include <stdio.h>

namespace TBCLI {
    namespace Util {
        enum Err {
            DIRECTORY_CREATION,
            DIRECTORY_LISTING,
            DIRECTORY_COMPRESSION,
            ARCHIVE_WRITE,
            ARCHIVE_READ
        };
        bool check_env();
        void init_env();
        bool check_dev(FILE *dev);
        bool check_dev_write_protection(FILE *dev);
        void init_dev(FILE *dev, bool write_protection);
        void set_dev_write_protection(FILE *dev);
        void get_dir();
        void compress_dir(char *dir);
        void write_archive(FILE *dev);
        void read_archive(FILE *dev);
    }
}

#endif
