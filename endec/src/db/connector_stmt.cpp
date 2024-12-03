#include <cstring>
#include <ctime>

#include "connector.h"

void TBENDEC::Connector::add(
    char *dev, char *file_name, char *hmac, char *key) const {
    int ret = sqlite3_bind_text(this->stmt_add, 1, dev, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_bind_text(this->stmt_add, 2, file_name, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_bind_blob(this->stmt_add, 3, hmac, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_bind_blob(this->stmt_add, 4, key, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_add);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_add);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

bool TBENDEC::Connector::check(char *dev) const {
    int ret = sqlite3_bind_text(this->stmt_check, 1, dev, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_check);
    if (ret != SQLITE_ROW) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    bool res = (sqlite3_column_int(this->stmt_check, 0) == 1);
    ret = sqlite3_step(this->stmt_check);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_check);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    return res;
}

void TBENDEC::Connector::update(
    char *dev, char *file_name, char *hmac, char *key) const {
    int ret = sqlite3_bind_text(this->stmt_update, 4, dev, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_bind_text(this->stmt_update, 1, file_name, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_bind_blob(this->stmt_update, 2, hmac, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_bind_blob(this->stmt_update, 3, key, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_update);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_update);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBENDEC::Connector::get_key(char *dev, char *key) const {
    int ret =
        sqlite3_bind_text(this->stmt_get_key, 1, dev, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_get_key);
    if (ret != SQLITE_ROW) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    const void *res =
        sqlite3_column_blob(this->stmt_get_key, 0);
    memcpy(key, res, 64);
    ret = sqlite3_step(this->stmt_get_key);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_get_key);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBENDEC::Connector::get_hmac(char *dev, char *hmac) const {
    int ret =
        sqlite3_bind_text(this->stmt_get_hmac, 1, dev, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_get_hmac);
    if (ret != SQLITE_ROW) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    const void *res =
        sqlite3_column_blob(this->stmt_get_hmac, 0);
    memcpy(hmac, res, 64);
    ret = sqlite3_step(this->stmt_get_hmac);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_get_hmac);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

std::string TBENDEC::Connector::get_file_name(char *dev) const {
    int ret =
        sqlite3_bind_text(this->stmt_get_file_name, 1, dev, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_get_file_name);
    if (ret != SQLITE_ROW) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    const unsigned char *res =
        sqlite3_column_text(this->stmt_get_file_name, 0);
    std::string fn = std::string((char*)res);
    ret = sqlite3_step(this->stmt_get_file_name);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_get_file_name);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    return fn;
}
