#include <unistd.h>
#include <cstring>

#include "app.h"
#include "../util/util.h"

TBCLI::App::App(char *dev_name) {
    puts("TBCLI");
    this->dev = fopen(dev_name, "r+");
    if (!this->dev) {
        throw Err::DEVICE_OPEN;
    }
    printf("Device %s opened\n", dev_name);
    puts("Checking environment");
    bool pass = TBCLI::Util::check_env();
    if (pass) {
        puts("Pass");
    } else {
        puts("~/backup not found, creating");
        TBCLI::Util::init_env();
        puts("~/backup created");
    }
}

TBCLI::App::~App() {
    if (this->dev) {
        fclose(this->dev);
    }
}
