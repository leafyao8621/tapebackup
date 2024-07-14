#include <iostream>
#include <cstring>

#include <unistd.h>

#include "app/app.h"

const static char *msg =
    "Usage: tbcli [OPTIONS] <MODE> <DEVICE>\n"
    "Arguments:\n"
    "<MODE>    [possible values: read, write]\n"
    "<DEVICE>\n\n"
    "Options:\n"
    "-p <PATH>\n"
    "-w\n"
    "-v";

int main(int argc, char **argv) {
    int ret = 0;
    bool path_set = false;
    std::string path = "";
    bool write_protect = false;
    bool read = false;
    bool write = false;
    bool verbose = false;
    for (; (ret = getopt(argc, argv, "p:wv")) != -1;) {
        switch (ret) {
        case 'p':
            path_set = true;
            path = optarg;
            break;
        case 'w':
            write_protect = true;
            break;
        case 'v':
            verbose = true;
            break;
        case ':':
            std::cerr << msg << std::endl;
            return -1;
        case '?':
            std::cerr << msg << std::endl;
            return -1;
        }
    }
    if (optind != argc - 2) {
        std::cerr << msg << std::endl;
        return -1;
    }
    if (!strcmp(argv[optind], "read")) {
        read = true;
    }
    if (!strcmp(argv[optind], "write")) {
        write = true;
    }
    if (!read && !write) {
        std::cerr << msg << std::endl;
        return -1;
    } else if (write && !path_set) {
        std::cerr << msg << std::endl;
        return -1;
    }
    std::string dev = argv[optind + 1];
    try {
        TBCLI::App app;
        if (write) {
            app.write(
                (char*)dev.c_str(),
                (char*)path.c_str(),
                write_protect,
                verbose
            );
        }
        if (read) {
            if (path_set) {
                app.read(
                    (char*)dev.c_str(),
                    (char*)path.c_str(),
                    verbose
                );
            }
        }
    } catch (TBCLI::App::Err err) {
        switch (err) {
        case TBCLI::App::Err::WRITE_PROTECT_TAMPERED:
            std::cerr << "APP WRITE_PROTECT_TAMPERED" << std::endl;
            break;
        case TBCLI::App::Err::WRITE_PROTECTED:
            std::cerr << "APP WRITE_PROTECTED" << std::endl;
            break;
        case TBCLI::App::Err::WRITE:
            std::cerr << "APP WRITE" << std::endl;
            break;
        case TBCLI::App::Err::INVALID_SIGNATURE:
            std::cerr << "APP INVALID_SIGNATURE" << std::endl;
            break;
        case TBCLI::App::Err::READ:
            std::cerr << "APP READ" << std::endl;
            break;
        case TBCLI::App::Err::FILE_TAMPERED:
            std::cerr << "APP FILE_TAMPERED" << std::endl;
            break;
        }
        return 1;
    } catch (TBCLI::Connector::Err err) {
        switch (err) {
        case TBCLI::Connector::Err::CONNECTION:
            std::cerr << "CONNECTOR CONNECTION" << std::endl;
            break;
        case TBCLI::Connector::Err::STMT_CREATION:
            std::cerr << "CONNECTOR STMT_CREATION" << std::endl;
            break;
        case TBCLI::Connector::Err::STMT_EXECUTION:
            std::cerr << "CONNECTOR STMT_EXECUTION" << std::endl;
            break;
        case TBCLI::Connector::Err::STMT_BIND:
            std::cerr << "CONNECTOR STMT_BIND" << std::endl;
            break;
        }
        return 2;
    } catch (TBCLI::Util::Err err) {
        switch (err) {
        case TBCLI::Util::Err::INVALID_DIR:
            std::cerr << "UTIL INVALID_DIR" << std::endl;
            break;
        case TBCLI::Util::Err::DEVICE_OPEN:
            std::cerr << "UTIL DEVICE_OPEN" << std::endl;
            break;
        case TBCLI::Util::Err::DEVICE_WRITE:
            std::cerr << "UTIL DEVICE_WRITE" << std::endl;
            break;
        case TBCLI::Util::Err::DIRECTORY_LISTING:
            std::cerr << "UTIL DIRECTORY_LISTING" << std::endl;
            break;
        case TBCLI::Util::Err::DIRECTORY_COMPRESSION:
            std::cerr << "UTIL DIRECTORY_COMPRESSION" << std::endl;
            break;
        case TBCLI::Util::Err::ARCHIVE_WRITE:
            std::cerr << "UTIL ARCHIVE_WRITE" << std::endl;
            break;
        case TBCLI::Util::Err::ARCHIVE_READ:
            std::cerr << "UTIL ARCHIVE_READ" << std::endl;
            break;
        case TBCLI::Util::Err::READONLY_FLAG:
            std::cerr << "UTIL READONLY_FLAG" << std::endl;
            break;
        }
        return 3;
    } catch (TBCLI::Util::Env::Err err) {
        switch (err) {
        case TBCLI::Util::Env::Err::DIRECTORY_CREATION:
            std::cerr << "ENV DIRECTORY_CREATION" << std::endl;
            break;
        }
        return 4;
    } catch (TBCLI::Util::Gen::Err err) {
        switch (err) {
        case TBCLI::Util::Gen::Err::OPEN:
            std::cerr << "GEN OPEN" << std::endl;
            break;
        case TBCLI::Util::Gen::Err::READ:
            std::cerr << "GEN READ" << std::endl;
            break;
        }
        return 4;
    } catch (TBCLI::Util::Archiver::Err err) {
        switch (err) {
        case TBCLI::Util::Archiver::Err::OPEN:
            std::cerr << "ARCHIVER OPEN" << std::endl;
            break;
        case TBCLI::Util::Archiver::Err::APPEND:
            std::cerr << "ARCHIVER APPEND" << std::endl;
            break;
        }
        return 5;
    } catch (TBCLI::Util::HMAC::Err err) {
        switch (err) {
        case TBCLI::Util::HMAC::Err::CREATION:
            std::cerr << "HMAC CREATION" << std::endl;
            break;
        case TBCLI::Util::HMAC::Err::INITIALIZATION:
            std::cerr << "HMAC INITIALIZATION" << std::endl;
            break;
        case TBCLI::Util::HMAC::Err::UPDATE:
            std::cerr << "HMAC UPDATE" << std::endl;
            break;
        case TBCLI::Util::HMAC::Err::FINALIZATION:
            std::cerr << "HMAC FINALIZATION" << std::endl;
            break;
        }
        return 6;
    } catch (TBCLI::Util::Writer::Err err) {
        switch (err) {
        case TBCLI::Util::Writer::Err::OPEN:
            std::cerr << "WRITER OPEN" << std::endl;
            break;
        case TBCLI::Util::Writer::Err::READ:
            std::cerr << "WRITER READ" << std::endl;
            break;
        case TBCLI::Util::Writer::Err::WRITE:
            std::cerr << "WRITER WRITE" << std::endl;
            break;
        }
        return 7;
    }
    return 0;
}
