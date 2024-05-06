#ifndef APP_APP_H_
#define APP_APP_H_

#include <cstdio>
#include <pthread.h>

namespace TBCLI {
    class App {
    public:
        enum Err {
            DEVICE_OPEN,
            DEVICE_NOT_INTIALIZED,
            DEVICE_WRITE_PROTECTED
        };
    private:
        FILE *dev;
    public:
        App(char *dev_name);
        ~App();
        void main_loop();
    private:
        void read();
        void write();
    };
}

#endif
