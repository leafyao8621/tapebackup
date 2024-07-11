#ifndef APP_APP_H_
#define APP_APP_H_

#include <vector>

#include "../db/connector.h"
#include "../util/util.h"

namespace TBCLI {
    class App {
    public:
        enum Err {
            DEVICE_OPEN,
            DEVICE_NOT_INTIALIZED,
            DEVICE_WRITE_PROTECTED,
            DEVICE_WRITE_PROTECTION_TAMPERED,
            DEVICE_FILE_TAMPERED
        };
    private:
        char *dev_name;
        Connector connector;
        TBCLI::Util::Gen gen;
        TBCLI::Util::Env env;
        TBCLI::Util::HMAC hmac;
    public:
        App(char *dev_name);
        void main_loop() const;
    private:
        void read_dev() const;
        void write_dev() const;
    };
}

#endif
