#ifndef DB_CONNECTOR_H_
#define DB_CONNECTOR_H_

#include <string>

#include <sqlite3.h>

#include "../report/report.h"

namespace TBCLI {
    class Connector {
        sqlite3 *conn;
        sqlite3_stmt *stmt_init;
        sqlite3_stmt *stmt_check;
        sqlite3_stmt *stmt_get_write_protection;
        sqlite3_stmt *stmt_add;
        sqlite3_stmt *stmt_set_write_protection;
        sqlite3_stmt *stmt_update_file_name;
        sqlite3_stmt *stmt_update_key;
        sqlite3_stmt *stmt_update_hmac;
        sqlite3_stmt *stmt_get_key;
        sqlite3_stmt *stmt_get_hmac;
        sqlite3_stmt *stmt_get_file_name;
        sqlite3_stmt *stmt_set_start_time;
        sqlite3_stmt *stmt_set_completion_time;
        sqlite3_stmt *stmt_set_reported_size;
        sqlite3_stmt *stmt_set_written_size;
        sqlite3_stmt *stmt_report_daily;
        void print_err() const;
    public:
        enum Err {
            CONNECTION,
            STMT_CREATION,
            STMT_EXECUTION,
            STMT_BIND
        };
        Connector();
        ~Connector();
        bool check(char *signature) const;
        bool get_write_protection(char *signature) const;
        void add(char *signature, bool write_protection) const;
        void set_write_protection(char *signature) const;
        void update_file_name(char *signature, char *file_name) const;
        void update_key(char *signature, char *key) const;
        void update_hmac(char *signature, char *hmac) const;
        void get_key(char *signature, char *key) const;
        void get_hmac(char *signature, char *hmac) const;
        std::string get_file_name(char *signature) const;
        void set_start_time(char *signature) const;
        void set_completion_time(char *signature) const;
        void set_reported_size(char *signature, size_t size) const;
        void set_written_size(char *signature, size_t size) const;
        void report_daily(
            std::string beginning,
            std::string ending,
            Report::Format format,
            std::ostream &os) const;
    };
}

#endif
