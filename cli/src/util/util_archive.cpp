#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include "util.h"

void TBCLI::Util::write_archive(char *dev_name) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    char res = 'n';
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res != 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
    std::ostringstream oss_dd_skip;
    oss_dd_skip <<
        "dd if=" <<
        dev_name <<
        " of=/dev/null bs=66 count=1";
    system(oss_dd_skip.str().c_str());
    std::ostringstream oss_dd;
    oss_dd <<
        "dd if=" <<
        getenv("HOME") << "/backup/backup.tar.gz" <<
        " of=" << dev_name <<
        " status=progress";
    system(oss_dd.str().c_str());
}

void TBCLI::Util::read_archive(char *dev_name) {
    std::ostringstream oss;
    oss << "mt -f " << dev_name << " rewind";
    system(oss.str().c_str());
    char res = 'n';
    std::cout << "Rewinded? [y/n]: ";
    std::cout.flush();
    for (std::cin >> res; res != 'y'; std::cin >> res) {
        std::cout << "Rewinded? [y/n]: ";
        std::cout.flush();
    }
    std::ostringstream oss_dd_skip;
    oss_dd_skip <<
        "dd if=" <<
        dev_name <<
        " of=/dev/null bs=66 count=1";
    system(oss_dd_skip.str().c_str());
    std::ostringstream oss_dd;
    oss_dd <<
        "dd if=" <<
        dev_name <<
        " of=" <<
        getenv("HOME") << "/backup/restore.tar.gz" <<
        " status=progress";
    system(oss_dd.str().c_str());
}
