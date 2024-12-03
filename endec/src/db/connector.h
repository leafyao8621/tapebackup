#ifndef DB_CONNECTOR_H_
#define DB_CONNECTOR_H_

#include <string>

#include <sqlite3.h>

namespace TBENDEC {
    class Connector {
        sqlite3 *conn;
        sqlite3_stmt *stmt_init;
        sqlite3_stmt *stmt_add;
        sqlite3_stmt *stmt_check;
        sqlite3_stmt *stmt_update;
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
        void add(char *dev, char *file_name, char *hmac, char *key) const;
        bool check(char *dev) const;
        void update(char *dev,char *file_name, char *hmac, char *key) const;
        void get_key(char *dev, char *key) const;
        void get_hmac(char *dev, char *key) const;
        std::string get_file_name(char *dev) const;
    };
}

#endif
