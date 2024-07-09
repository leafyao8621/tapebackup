#include <iostream>

#include "app/app.h"
#include "util/util.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: tbtui <device name>" << std::endl;
        return 0;
    }
    try {
        TBTUI::App app(argv[1]);
        app.run();
    } catch (TBTUI::App::Err err) {
        switch (err) {
        case TBTUI::App::Err::INIT:
            std::cout << "APP INIT" << std::endl;
            break;
        case TBTUI::App::Err::FOLDER_CREATION:
            std::cout << "Cannot create ~/backup" << std::endl;
            break;
        }
    } catch (TBTUI::Connector::Err) {
        std::cout << "DB INIT" << std::endl;
    } catch (TBTUI::Util::Gen::Err) {
        std::cout << "GEN INIT" << std::endl;
    }
    // } catch (TBTUI::Util::Err err) {
    //     switch (err) {
    //     case TBTUI::Util::Err::DEVICE_OPEN:
    //         std::cout << "Cannot open " << argv[1] << std::endl;
    //         break;
    //     default:
    //         break;
    //     }
    // }
    return 0;
}
