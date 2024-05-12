#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include "util.h"

void TBCLI::Util::write_archive(char *dev_name) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    std::ostringstream oss_dd_skip;
    oss_dd_skip <<
        "mt -f " <<
        dev_name <<
        " fsf 1";
    system(oss_dd_skip.str().c_str());
    std::ostringstream oss_dd;
    oss_dd <<
        "dd if=" <<
        getenv("HOME") << "/backup/backup.tar" <<
        " of=" <<
        dev_name <<
        " bs=10K status=progress";
    system(oss_dd.str().c_str());
}

void TBCLI::Util::read_archive(char *dev_name) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    std::ostringstream oss_dd_skip;
    oss_dd_skip <<
        "mt -f " <<
        dev_name <<
        " fsf 1";
    system(oss_dd_skip.str().c_str());
    std::ostringstream oss_dd;
    oss_dd <<
        "dd if=" <<
        dev_name <<
        " of=" <<
        getenv("HOME") << "/backup/restore.tar" <<
        " bs=10K status=progress";
    system(oss_dd.str().c_str());
}
