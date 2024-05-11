#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "app.h"
#include "../util/util.h"

TBCLI::App::App(char *dev_name) {
    std::cout << "TBCLI" << std::endl;
    this->dev_name = dev_name;
    std::cout << "Checking environment" << std::endl;
    TBCLI::Util::Env env;
    bool pass = env.check();
    if (pass) {
        std::cout << "Pass" << std::endl;
    } else {
        std::cout << "Initializing environment" << std::endl;
        env.init();
        std::cout << "Environment initialized" << std::endl;
    }
    std::cout << "Initializing signature" << std::endl;
    this->signature.init();
}
