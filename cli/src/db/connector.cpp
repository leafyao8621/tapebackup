#include <iostream>
#include <sstream>

#include "connector.h"

TBCLI::Connector::Connector() {
    std::ostringstream oss;
    oss << getenv("HOME") << "/.tb.db";
    int ret = sqlite3_open(oss.str().c_str(), &this->conn);
    if (ret) {
        this->print_err();
        throw Err::CONNECTION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "CREATE TABLE IF NOT EXISTS MAIN ( "
            "SIGNATURE BLOB(64) PRIMARY KEY,"
            "WRITE_PROTECTION INTEGER,"
            "HMAC_KEY BLOB(64),"
            "FILE_NAME TEXT,"
            "HMAC_VALUE BLOB(64)"
            ")",
            -1,
            &this->stmt_init,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
    ret = sqlite3_step(this->stmt_init);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "SELECT COUNT(SIGNATURE) FROM MAIN WHERE SIGNATURE = ?",
            -1,
            &this->stmt_check,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "SELECT WRITE_PROTECTION FROM MAIN WHERE SIGNATURE = ?",
            -1,
            &this->stmt_get_write_protection,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "INSERT INTO MAIN VALUES (?, ?, NULL, NULL, NULL)",
            -1,
            &this->stmt_add,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "UPDATE MAIN SET WRITE_PROTECTION = ? WHERE SIGNATURE = ?",
            -1,
            &this->stmt_set_write_protection,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "UPDATE MAIN SET HMAC_KEY = ? WHERE SIGNATURE = ?",
            -1,
            &this->stmt_update_key,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "UPDATE MAIN "
            "SET "
            "FILE_NAME = ?, "
            "HMAC_VALUE = ? "
            "WHERE "
            "SIGNATURE = ?",
            -1,
            &this->stmt_update_file,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
}

TBCLI::Connector::~Connector() {
    sqlite3_finalize(this->stmt_init);
    sqlite3_finalize(this->stmt_check);
    sqlite3_finalize(this->stmt_get_write_protection);
    sqlite3_finalize(this->stmt_add);
    sqlite3_finalize(this->stmt_set_write_protection);
    sqlite3_finalize(this->stmt_update_key);
    sqlite3_finalize(this->stmt_update_file);
    sqlite3_close(this->conn);
}

void TBCLI::Connector::print_err() const {
    std::cout << sqlite3_errmsg(this->conn) << std::endl;
}
