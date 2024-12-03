#include <iostream>
#include <sstream>

#include "connector.h"

TBENDEC::Connector::Connector() {
    std::ostringstream oss;
    oss << getenv("HOME") << "/.tbendec.db";
    int ret = sqlite3_open(oss.str().c_str(), &this->conn);
    if (ret) {
        this->print_err();
        throw Err::CONNECTION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "CREATE TABLE IF NOT EXISTS MAIN ("
            "DEV TEXT PRIMARY KEY,"
            "FILE_NAME TEXT,"
            "HMAC BLOB(64),"
            "HMAC_KEY BLOB(64)"
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
            "INSERT INTO MAIN VALUES "
            "(?, ?, ?, ?)",
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
            "SELECT COUNT(DEV) FROM MAIN WHERE DEV = ?",
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
            "UPDATE MAIN SET "
            "FILE_NAME = ?,"
            "HMAC = ?,"
            "HMAC_KEY = ? "
            "WHERE "
            "DEV = ?",
            -1,
            &this->stmt_update,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "SELECT HMAC_KEY FROM MAIN WHERE DEV = ?",
            -1,
            &this->stmt_get_key,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "SELECT HMAC FROM MAIN WHERE DEV = ?",
            -1,
            &this->stmt_get_hmac,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
    ret =
        sqlite3_prepare_v2(
            this->conn,
            "SELECT FILE_NAME FROM MAIN WHERE DEV = ?",
            -1,
            &this->stmt_get_file_name,
            NULL
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_CREATION;
    }
}

TBENDEC::Connector::~Connector() {
    sqlite3_finalize(this->stmt_init);
    sqlite3_finalize(this->stmt_add);
    sqlite3_finalize(this->stmt_check);
    sqlite3_finalize(this->stmt_update);
    sqlite3_finalize(this->stmt_get_key);
    sqlite3_finalize(this->stmt_get_hmac);
    sqlite3_finalize(this->stmt_get_file_name);
    sqlite3_close(this->conn);
}

void TBENDEC::Connector::print_err() const {
    std::cerr << sqlite3_errmsg(this->conn) << std::endl;
}
