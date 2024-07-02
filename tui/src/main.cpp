#include <iostream>

#include "app/app.h"


int main() {
    try {
        tbtui::App app;
        app.run();
    } catch (tbtui::App::Err err) {
        switch (err) {
        case tbtui::App::Err::INIT:
            std::cout << "APP INIT" << std::endl;
            break;
        }
    }
    return 0;
}
