#include <iostream>

#include "app/app.h"
#include "util/util.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: tbcli <device name>" << std::endl;
        return 0;
    }
    try {
        TBCLI::App app(argv[1]);
        app.main_loop();
    } catch (TBCLI::App::Err err) {
        switch (err) {
        case TBCLI::App::Err::DEVICE_OPEN:
            std::cout << "Cannot open device " << argv[1] << std::endl;
            break;
        case TBCLI::App::Err::DEVICE_NOT_INTIALIZED:
            std::cout << "Device " << argv[1] <<
                " not initialized" << std::endl;
            break;
        case TBCLI::App::Err::DEVICE_WRITE_PROTECTED:
            std::cout << "Device " << argv[1] <<
                " write proctected" << std::endl;
            break;
        case TBCLI::App::Err::DEVICE_WRITE_PROTECTION_TAMPERED:
            std::cout << "Device " << argv[1] <<
                " write proctection tampered" << std::endl;
            break;
        case TBCLI::App::Err::DEVICE_FILE_TAMPERED:
            std::cout << "Device " << argv[1] <<
                " file tampered" << std::endl;
            break;
        }
    } catch (TBCLI::Util::Err err) {
        switch (err) {
        case TBCLI::Util::DEVICE_OPEN:
            std::cout << "Failed to open device " << argv[1] << std::endl;
            break;
        case TBCLI::Util::DEVICE_WRITE:
            std::cout << "Failed to write to device " << argv[1] << std::endl;
            break;
        case TBCLI::Util::Err::DIRECTORY_LISTING:
            std::cout << "~/backup not accessible" << std::endl;
            break;
        case TBCLI::Util::Err::DIRECTORY_COMPRESSION:
            std::cout << "Compression failed" << std::endl;
            break;
        case TBCLI::Util::Err::ARCHIVE_WRITE:
            std::cout << "Archive write failed" << std::endl;
            break;
        case TBCLI::Util::Err::ARCHIVE_READ:
            std::cout << "Archive read failed" << std::endl;
            break;
        case TBCLI::Util::Err::READONLY_FLAG:
            std::cout << "Read only flag invalid" << std::endl;
            break;
        }
    } catch (TBCLI::Util::Env::Err err) {
        switch (err) {
        case TBCLI::Util::Env::Err::DIRECTORY_CREATION:
            std::cout << "~/backup creation failed" << std::endl;
            break;
        }
    } catch (TBCLI::Util::Gen::Err err) {
        switch (err) {
        case TBCLI::Util::Gen::Err::OPEN:
            std::cout << "Failed to open /dev/urandom" << std::endl;
            break;
        case TBCLI::Util::Gen::Err::READ:
            std::cout << "Failed to read from /dev/urandom" << std::endl;
            break;
        }
    } catch (TBCLI::Connector::Err err) {
        switch (err) {
        case TBCLI::Connector::Err::CONNECTION:
            std::cout << "Failed to connect to ~/.tb.db" << std::endl;
            break;
        case TBCLI::Connector::Err::STMT_CREATION:
            std::cout << "Failed to create statement" << std::endl;
            break;
        case TBCLI::Connector::Err::STMT_EXECUTION:
            std::cout << "Failed to execute statement" << std::endl;
            break;
        case TBCLI::Connector::Err::STMT_BIND:
            std::cout << "Failed to bind statement" << std::endl;
            break;
        }
    }
    return 0;
}
