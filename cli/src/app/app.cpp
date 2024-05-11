#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include "app.h"
#include "../util/util.h"

TBCLI::App::App(char *dev_name) {
    puts("TBCLI");
    this->dev_name = dev_name;
    this->dev = open(dev_name, O_RDWR);
    if (!this->dev) {
        throw Err::DEVICE_OPEN;
    }
    printf("Device %s opened\n", dev_name);
    puts("Checking environment");
    TBCLI::Util::Env env;
    bool pass = env.check();
    if (pass) {
        puts("Pass");
    } else {
        puts("Initializing environment");
        env.init();
        puts("Environment initialized");
    }
    puts("Initializing signature");
    this->signature.init();
}

TBCLI::App::~App() {
    if (this->dev) {
        close(this->dev);
    }
}
