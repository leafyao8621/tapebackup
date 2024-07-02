#include <iostream>

#include "app/app.h"
#include "util/util.h"

int main() {
    try {
        std::string path = getenv("HOME");
        TBTUI::App app;
        app.run();
    } catch (TBTUI::App::Err err) {
        switch (err) {
        case TBTUI::App::Err::INIT:
            std::cout << "APP INIT" << std::endl;
            break;
        }
    }
    return 0;
}
