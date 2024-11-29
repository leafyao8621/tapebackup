#include <iostream>
#include <fstream>
#include <cstring>

#include <unistd.h>

#include "app/app.h"

const static char *msg =
    "Usage: tbcli [OPTIONS] <MODE> <DEVICE/REPORT>\n"
    "Arguments:\n"
    "<MODE>    [possible values: read, write, reset, report]\n"
    "<DEVICE/REPORT>\n"
    "[possible values for report: daily, list, lookup, transaction]\n\n"
    "Options:\n"
    "-p <PATH>\n"
    "-i <READ BLOCK SIZE>\n"
    "-o <WRITE BLOCK SIZE>\n"
    "-h <HMAC BLOCK SIZE>\n"
    "-a <ARCHIVE BLOCK SIZE>\n"
    "-b <BEGINNING DATE>\n"
    "-e <ENDING DATE>\n"
    "-f <FORMAT> [possible values: TEXT, CSV]\n"
    "-x <EXPORT FILE NAME>\n"
    "-s <SIGNATURE>\n"
    "-w\n"
    "-v";

int main(int argc, char **argv) {
    int ret = 0;
    bool path_set = false;
    bool read_set = false;
    bool write_set = false;
    bool hmac_set = false;
    bool archive_set = false;
    size_t block_size_read = 0;
    size_t block_size_write = 0;
    size_t block_size_hmac = 0;
    size_t block_size_archive = 0;
    std::string path = "";
    bool encrypt = false;
    bool decrypt = false;
    bool verbose = false;
    for (; (ret = getopt(argc, argv, "p:i:o:h:a:v")) != -1;) {
        switch (ret) {
        case 'p':
            path_set = true;
            path = optarg;
            break;
        case 'i':
            read_set = true;
            block_size_read = atol(optarg);
            if (block_size_read == (size_t)-1) {
                std::cerr << msg << std::endl;
                return -1;
            }
            break;
        case 'o':
            write_set = true;
            block_size_write = atol(optarg);
            if (block_size_write == (size_t)-1) {
                std::cerr << msg << std::endl;
                return -1;
            }
            break;
        case 'h':
            hmac_set = true;
            block_size_hmac = atol(optarg);
            if (block_size_hmac == (size_t)-1) {
                std::cerr << msg << std::endl;
                return -1;
            }
            break;
        case 'a':
            archive_set = true;
            block_size_archive = atol(optarg);
            if (block_size_archive == (size_t)-1) {
                std::cerr << msg << std::endl;
                return -1;
            }
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
    if (path[path.size() - 1] == '/') {
        path = path.substr(0, path.size() - 1);
    }
    if (!strcmp(argv[optind], "decrypt")) {
        decrypt = true;
    }
    if (!strcmp(argv[optind], "encrypt")) {
        encrypt = true;
    }
    if (!decrypt && !encrypt) {
        std::cerr << msg << std::endl;
        return -1;
    } else if (encrypt && !path_set) {
        std::cerr << msg << std::endl;
        return -1;
    }
    try {
        TBENDEC::App app;
        if (encrypt) {
            std::string dev = argv[optind + 1];
            if (read_set) {
                app.set_block_size_read(block_size_read);
            }
            if (write_set) {
                app.set_block_size_write(block_size_write);
            }
            if (hmac_set) {
                app.set_block_size_hmac(block_size_hmac);
            }
            if (archive_set) {
                app.set_block_size_archive(block_size_archive);
            }
            app.encrypt(
                (char*)dev.c_str(),
                (char*)path.c_str(),
                verbose
            );
        }
        if (decrypt) {
            std::string dev = argv[optind + 1];
            if (read_set) {
                app.set_block_size_read(block_size_read);
            }
            if (write_set) {
                app.set_block_size_write(block_size_write);
            }
            if (hmac_set) {
                app.set_block_size_hmac(block_size_hmac);
            }
            if (archive_set) {
                app.set_block_size_archive(block_size_archive);
            }
            app.decrypt(
                (char*)dev.c_str(),
                (char*)path.c_str(),
                verbose
            );
        }
    } catch (TBENDEC::App::Err err) {
        switch (err) {
        case TBENDEC::App::Err::FILE_TAMPERED:
            std::cerr << "APP FILE_TAMPERED" << std::endl;
            break;
        case TBENDEC::App::Err::WRITE:
            std::cerr << "APP WRITE" << std::endl;
            break;
        case TBENDEC::App::Err::READ:
            std::cerr << "APP READ" << std::endl;
            break;
        }
        return 1;
    } catch (TBENDEC::Connector::Err err) {
        switch (err) {
        case TBENDEC::Connector::Err::CONNECTION:
            std::cerr << "CONNECTOR CONNECTION" << std::endl;
            break;
        case TBENDEC::Connector::Err::STMT_CREATION:
            std::cerr << "CONNECTOR STMT_CREATION" << std::endl;
            break;
        case TBENDEC::Connector::Err::STMT_EXECUTION:
            std::cerr << "CONNECTOR STMT_EXECUTION" << std::endl;
            break;
        case TBENDEC::Connector::Err::STMT_BIND:
            std::cerr << "CONNECTOR STMT_BIND" << std::endl;
            break;
        }
        return 2;
    } catch (TBENDEC::Util::Err err) {
        switch (err) {
        case TBENDEC::Util::Err::INVALID_DIR:
            std::cerr << "UTIL INVALID_DIR" << std::endl;
            break;
        case TBENDEC::Util::Err::DEVICE_OPEN:
            std::cerr << "UTIL DEVICE_OPEN" << std::endl;
            break;
        case TBENDEC::Util::Err::DEVICE_WRITE:
            std::cerr << "UTIL DEVICE_WRITE" << std::endl;
            break;
        case TBENDEC::Util::Err::DIRECTORY_LISTING:
            std::cerr << "UTIL DIRECTORY_LISTING" << std::endl;
            break;
        case TBENDEC::Util::Err::DIRECTORY_COMPRESSION:
            std::cerr << "UTIL DIRECTORY_COMPRESSION" << std::endl;
            break;
        case TBENDEC::Util::Err::ARCHIVE_WRITE:
            std::cerr << "UTIL ARCHIVE_WRITE" << std::endl;
            break;
        case TBENDEC::Util::Err::ARCHIVE_READ:
            std::cerr << "UTIL ARCHIVE_READ" << std::endl;
            break;
        case TBENDEC::Util::Err::READONLY_FLAG:
            std::cerr << "UTIL READONLY_FLAG" << std::endl;
            break;
        }
        return 3;
    } catch (TBENDEC::Util::Env::Err err) {
        switch (err) {
        case TBENDEC::Util::Env::Err::DIRECTORY_CREATION:
            std::cerr << "ENV DIRECTORY_CREATION" << std::endl;
            break;
        }
        return 4;
    } catch (TBENDEC::Util::Gen::Err err) {
        switch (err) {
        case TBENDEC::Util::Gen::Err::OPEN:
            std::cerr << "GEN OPEN" << std::endl;
            break;
        case TBENDEC::Util::Gen::Err::READ:
            std::cerr << "GEN READ" << std::endl;
            break;
        }
        return 4;
    } catch (TBENDEC::Util::Archiver::Err err) {
        switch (err) {
        case TBENDEC::Util::Archiver::Err::OPEN:
            std::cerr << "ARCHIVER OPEN" << std::endl;
            break;
        case TBENDEC::Util::Archiver::Err::APPEND:
            std::cerr << "ARCHIVER APPEND" << std::endl;
            break;
        }
        return 5;
    } catch (TBENDEC::Util::HMAC::Err err) {
        switch (err) {
        case TBENDEC::Util::HMAC::Err::CREATION:
            std::cerr << "HMAC CREATION" << std::endl;
            break;
        case TBENDEC::Util::HMAC::Err::INITIALIZATION:
            std::cerr << "HMAC INITIALIZATION" << std::endl;
            break;
        case TBENDEC::Util::HMAC::Err::UPDATE:
            std::cerr << "HMAC UPDATE" << std::endl;
            break;
        case TBENDEC::Util::HMAC::Err::FINALIZATION:
            std::cerr << "HMAC FINALIZATION" << std::endl;
            break;
        }
        return 6;
    } catch (TBENDEC::Util::Writer::Err err) {
        switch (err) {
        case TBENDEC::Util::Writer::Err::OPEN:
            std::cerr << "WRITER OPEN" << std::endl;
            break;
        case TBENDEC::Util::Writer::Err::READ:
            std::cerr << "WRITER READ" << std::endl;
            break;
        case TBENDEC::Util::Writer::Err::WRITE:
            std::cerr << "WRITER WRITE" << std::endl;
            break;
        case TBENDEC::Util::Writer::Err::CIPHER_INIT:
            std::cerr << "WRITER CIPHER_INIT" << std::endl;
            break;
        case TBENDEC::Util::Writer::Err::CIPHER_UPDATE:
            std::cerr << "WRITER CIPHER_UPDATE" << std::endl;
            break;
        case TBENDEC::Util::Writer::Err::CIPHER_FINAL:
            std::cerr << "WRITER CIPHER_FINAL" << std::endl;
            break;
        }
        return 7;
    }  catch (TBENDEC::Util::Reader::Err err) {
        switch (err) {
        case TBENDEC::Util::Reader::Err::OPEN:
            std::cerr << "Reader OPEN" << std::endl;
            break;
        case TBENDEC::Util::Reader::Err::READ:
            std::cerr << "Reader READ" << std::endl;
            break;
        case TBENDEC::Util::Reader::Err::WRITE:
            std::cerr << "Reader WRITE" << std::endl;
            break;
        case TBENDEC::Util::Reader::Err::CIPHER_INIT:
            std::cerr << "Reader CIPHER_INIT" << std::endl;
            break;
        case TBENDEC::Util::Reader::Err::CIPHER_UPDATE:
            std::cerr << "Reader CIPHER_UPDATE" << std::endl;
            break;
        case TBENDEC::Util::Reader::Err::CIPHER_FINAL:
            std::cerr << "Reader CIPHER_FINAL" << std::endl;
            break;
        }
        return 8;
    }
    return 0;
}
