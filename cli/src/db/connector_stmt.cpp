#include "connector.h"

bool TBCLI::Connector::check(char *signature) const {
    int ret = sqlite3_bind_blob(this->stmt_check, 1, signature, 64, 0);
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
    return res;
}

bool TBCLI::Connector::get_write_protection(char *signature) const {
    int ret =
        sqlite3_bind_blob(this->stmt_get_write_protection, 1, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_get_write_protection);
    if (ret != SQLITE_ROW) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    bool res = sqlite3_column_int(this->stmt_get_write_protection, 0);
    ret = sqlite3_step(this->stmt_get_write_protection);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    return res;
}

void TBCLI::Connector::add(char *signature, bool write_protection) const {
    int ret = sqlite3_bind_blob(this->stmt_add, 1, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_bind_int(this->stmt_add, 2, write_protection);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_add);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::set_write_protection(char *signature) const {
    int ret = sqlite3_bind_int(this->stmt_set_write_protection, 1, true);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_set_write_protection, 2, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_set_write_protection);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}
