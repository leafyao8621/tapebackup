#ifndef DB_CONNECTOR_H_
#define DB_CONNECTOR_H_

#include <string>
#include <sqlite3.h>

namespace TBTUI {
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
    };
}

#endif
