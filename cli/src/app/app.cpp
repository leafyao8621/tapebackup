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
    bool pass = this->env.check();
    if (pass) {
        std::cout << "Pass" << std::endl;
    } else {
        std::cout << "Initializing environment" << std::endl;
        this->env.init();
        std::cout << "Environment initialized" << std::endl;
    }
}
