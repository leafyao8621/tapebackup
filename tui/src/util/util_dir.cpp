#include <sys/stat.h>
#include <dirent.h>
#include <cstdlib>
#include <sstream>

#include "util.h"

void TBTUI::Util::get_dir(std::string path, std::vector<std::string> &listing) {
    DIR *dp;
    struct dirent *ep;
    dp = opendir(path.c_str());
    for (; (ep = readdir(dp));) {
        if (ep->d_type == DT_DIR) {
            if (*ep->d_name == '.') {
                continue;
            }
            listing.push_back(ep->d_name);
        }
    }
    closedir(dp);
}
