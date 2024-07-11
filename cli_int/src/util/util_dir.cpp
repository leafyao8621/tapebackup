#include <sys/stat.h>
#include <dirent.h>
#include <cstdlib>
#include <sstream>

#include "util.h"

void TBCLI::Util::get_dir() {
    std::ostringstream oss;
    oss << getenv("HOME") << "/backup";
    DIR *dp;
    struct dirent *ep;
    dp = opendir(oss.str().c_str());
    if (!dp) {
        throw Err::DIRECTORY_LISTING;
    }
    for (; (ep = readdir(dp));) {
        if (ep->d_type == DT_DIR) {
            if (*ep->d_name == '.') {
                continue;
            }
            puts(ep->d_name);
        }
    }
    closedir(dp);
}

void TBCLI::Util::compress_dir(char *dir) {
    std::ostringstream oss_dir;
    oss_dir << getenv("HOME") << "/backup/" << dir;
    std::ostringstream oss_archive;
    oss_archive << getenv("HOME") << "/backup/archive/" << dir << ".tar";
    struct stat st;
    if (stat(oss_dir.str().c_str(), &st)) {
        throw Err::DIRECTORY_COMPRESSION;
    }
    if (!S_ISDIR(st.st_mode)) {
        throw Err::DIRECTORY_COMPRESSION;
    }
    std::ostringstream oss_command;
    oss_command <<
        "tar cvf " << oss_archive.str().c_str() << " -C " <<
        oss_dir.str() << " .";
    int ret = system(oss_command.str().c_str());
    if (ret) {
        throw Err::DIRECTORY_COMPRESSION;
    }
}
