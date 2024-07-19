#include <iostream>
#include <fstream>
#include <cstring>

#include <unistd.h>

#include "app/app.h"

const static char *msg =
    "Usage: tbcli [OPTIONS] <MODE> <DEVICE/REPORT>\n"
    "Arguments:\n"
    "<MODE>    [possible values: read, write, report]\n"
    "<DEVICE/REPORT> [possible values for report: daily, list, lookup]\n\n"
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
    "-w\n"
    "-v";

int main(int argc, char **argv) {
    int ret = 0;
    bool path_set = false;
    bool read_set = false;
    bool write_set = false;
    bool hmac_set = false;
    bool archive_set = false;
    bool beginning_set = false;
    bool ending_set = false;
    bool format_set = false;
    bool export_file_name_set = false;
    size_t block_size_read = 0;
    size_t block_size_write = 0;
    size_t block_size_hmac = 0;
    size_t block_size_archive = 0;
    std::string path = "";
    std::string beginning = "";
    std::string ending = "";
    std::string format = "";
    std::string export_file_name = "";
    bool write_protect = false;
    bool read = false;
    bool write = false;
    bool report = false;
    bool verbose = false;
    for (; (ret = getopt(argc, argv, "p:i:o:h:a:b:e:f:x:wv")) != -1;) {
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
        case 'b':
            beginning_set = true;
            beginning = optarg;
            break;
        case 'e':
            ending_set = true;
            ending = optarg;
            break;
        case 'f':
            format_set = true;
            format = optarg;
            break;
        case 'x':
            export_file_name_set = true;
            export_file_name = optarg;
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
    if (path[path.size() - 1] == '/') {
        path = path.substr(0, path.size() - 1);
    }
    if (!strcmp(argv[optind], "read")) {
        read = true;
    }
    if (!strcmp(argv[optind], "write")) {
        write = true;
    }
    if (!strcmp(argv[optind], "report")) {
        report = true;
    }
    if (!read && !write && !report) {
        std::cerr << msg << std::endl;
        return -1;
    } else if (write && !path_set) {
        std::cerr << msg << std::endl;
        return -1;
    }

    try {
        TBCLI::App app;
        if (write) {
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
            app.write(
                (char*)dev.c_str(),
                (char*)path.c_str(),
                write_protect,
                verbose
            );
        }
        if (read) {
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
            if (path_set) {
                app.read(
                    (char*)dev.c_str(),
                    (char*)path.c_str(),
                    verbose
                );
            } else {
                app.read(
                    (char*)dev.c_str(),
                    verbose
                );
            }
        }
        if (report) {
            if (!strcmp(argv[optind + 1], "daily")) {
                if (!beginning_set) {
                    beginning = "1900-01-01";
                }
                if (!ending_set) {
                    ending = "2100-01-01";
                }
                TBCLI::Report::Format format_enum = TBCLI::Report::Format::TEXT;
                if (format_set) {
                    if (!strcmp(format.c_str(), "CSV")) {
                        format_enum = TBCLI::Report::Format::CSV;
                    }
                }
                if (export_file_name_set) {
                    std::ofstream ofs(export_file_name);
                    app.report_daily(beginning, ending, format_enum, ofs);
                } else {
                    app.report_daily(beginning, ending, format_enum, std::cout);
                }
            }
            if (!strcmp(argv[optind + 1], "list")) {
                if (!beginning_set) {
                    beginning = "1900-01-01";
                }
                if (!ending_set) {
                    ending = "2100-01-01";
                }
                TBCLI::Report::Format format_enum = TBCLI::Report::Format::TEXT;
                if (format_set) {
                    if (!strcmp(format.c_str(), "CSV")) {
                        format_enum = TBCLI::Report::Format::CSV;
                    }
                }
                if (export_file_name_set) {
                    std::ofstream ofs(export_file_name);
                    app.report_list(beginning, ending, format_enum, ofs);
                } else {
                    app.report_list(beginning, ending, format_enum, std::cout);
                }
            }
            if (!strcmp(argv[optind + 1], "lookup")) {
                if (!path_set) {
                    std::cerr << msg << std::endl;
                    return -1;
                }
                TBCLI::Report::Format format_enum = TBCLI::Report::Format::TEXT;
                if (format_set) {
                    if (!strcmp(format.c_str(), "CSV")) {
                        format_enum = TBCLI::Report::Format::CSV;
                    }
                }
                if (export_file_name_set) {
                    std::ofstream ofs(export_file_name);
                    app.report_lookup(path, format_enum, ofs);
                } else {
                    app.report_lookup(path, format_enum, std::cout);
                }
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
    } catch (TBCLI::Report::Err err) {
        switch (err) {
        case TBCLI::Report::Err::STMT_EXECUTION:
            std::cerr << "REPORT STMT_EXECUTION" << std::endl;
            break;
        }
        return 7;
    }
    return 0;
}
