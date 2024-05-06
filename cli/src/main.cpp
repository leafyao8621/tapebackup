#include "app/app.h"
#include "util/util.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        puts("Usage: tbcli <device name>");
        return 0;
    }
    try {
        TBCLI::App app(argv[1]);
        app.main_loop();
    } catch (TBCLI::App::Err err) {
        switch (err) {
        case TBCLI::App::Err::DEVICE_OPEN:
            printf("Cannot open device %s\n", argv[1]);
            break;
        case TBCLI::App::Err::DEVICE_NOT_INTIALIZED:
            printf("Device %s not initialized\n", argv[1]);
            break;
        case TBCLI::App::Err::DEVICE_WRITE_PROTECTED:
            printf("Device %s write proctected\n", argv[1]);
            break;
        }
    } catch (TBCLI::Util::Err err) {
        switch (err) {
        case TBCLI::Util::Err::DIRECTORY_CREATION:
            puts("~/backup creation failed");
            break;
        case TBCLI::Util::Err::DIRECTORY_LISTING:
            puts("~/backup not accessible");
            break;
        case TBCLI::Util::Err::DIRECTORY_COMPRESSION:
            puts("Compression failed");
            break;
        case TBCLI::Util::Err::ARCHIVE_WRITE:
            puts("Archive write failed");
            break;
        case TBCLI::Util::Err::ARCHIVE_READ:
            puts("Archive read failed");
            break;
        }
    }
    return 0;
}
