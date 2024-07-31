#include <cstring>
#include <ctime>

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
    ret = sqlite3_reset(this->stmt_check);
    if (ret) {
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
    ret = sqlite3_reset(this->stmt_get_write_protection);
    if (ret) {
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
    ret = sqlite3_reset(this->stmt_add);
    if (ret) {
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
    ret = sqlite3_reset(this->stmt_set_write_protection);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::reset_write_protection(char *signature) const {
    int ret = sqlite3_bind_int(this->stmt_set_write_protection, 1, false);
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
    ret = sqlite3_reset(this->stmt_set_write_protection);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::update_file_name(
    char *signature, char *file_name) const {
    int ret =
        sqlite3_bind_text(this->stmt_update_file_name, 1, file_name, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_update_file_name, 2, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_update_file_name);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_update_file_name);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::update_key(char *signature, char *key) const {
    int ret =
        sqlite3_bind_blob(this->stmt_update_key, 1, key, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_update_key, 2, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_update_key);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_update_key);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::update_hmac(char *signature, char *hmac) const {
    int ret =
        sqlite3_bind_blob(this->stmt_update_hmac, 1, hmac, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_update_hmac, 2, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_update_hmac);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_update_hmac);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::get_key(char *signature, char *key) const {
    int ret =
        sqlite3_bind_blob(this->stmt_get_key, 1, signature, 64, 0);
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

void TBCLI::Connector::get_hmac(char *signature, char *hmac) const {
    int ret =
        sqlite3_bind_blob(this->stmt_get_hmac, 1, signature, 64, 0);
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

std::string TBCLI::Connector::get_file_name(char *signature) const {
    int ret =
        sqlite3_bind_blob(this->stmt_get_file_name, 1, signature, 64, 0);
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

void TBCLI::Connector::set_start_time(
    char *signature, time_t start_time) const {
    int ret =
        sqlite3_bind_int64(this->stmt_set_start_time, 1, start_time);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_set_start_time, 2, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_set_start_time);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_set_start_time);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::set_completion_time(
    char *signature, time_t completion_time) const {
    int ret =
        sqlite3_bind_int64(this->stmt_set_completion_time, 1, completion_time);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_set_completion_time, 2, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_set_completion_time);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_set_completion_time);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::set_reported_size(char *signature, size_t size) const {
    int ret =
        sqlite3_bind_int64(this->stmt_set_reported_size, 1, size);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_set_reported_size, 2, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_set_reported_size);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_set_reported_size);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::set_written_size(char *signature, size_t size) const {
    int ret =
        sqlite3_bind_int64(this->stmt_set_written_size, 1, size);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_set_written_size, 2, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_set_written_size);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_set_written_size);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::add_transaction(
    char *signature,
    bool write_protection,
    char *hmac_key,
    char *file_name,
    char *hmac_value,
    time_t start_time,
    time_t completion_time,
    size_t reported_size,
    size_t written_size) const {
    int ret =
        sqlite3_bind_blob(this->stmt_add_transaction, 1, signature, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_int(this->stmt_add_transaction, 2, write_protection);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_add_transaction, 3, hmac_key, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_text(this->stmt_add_transaction, 4, file_name, -1, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_blob(this->stmt_add_transaction, 5, hmac_value, 64, 0);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_int64(this->stmt_add_transaction, 6, start_time);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_int64(this->stmt_add_transaction, 7, completion_time);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_int64(this->stmt_add_transaction, 8, reported_size);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_int64(this->stmt_add_transaction, 9, written_size);
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret = sqlite3_step(this->stmt_add_transaction);
    if (ret != SQLITE_DONE) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
    ret = sqlite3_reset(this->stmt_add_transaction);
    if (ret) {
        this->print_err();
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::Connector::report_daily(
    std::string beginning,
    std::string ending,
    Report::Format format,
    std::ostream &os) const {
    int ret =
        sqlite3_bind_text(
            this->stmt_report_daily,
            1,
            beginning.c_str(),
            beginning.size(),
            0
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_text(
            this->stmt_report_daily,
            2,
            ending.c_str(),
            ending.size(),
            0
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ReportDaily report;
    report(this->stmt_report_daily, format, os);
}

void TBCLI::Connector::report_list(
    std::string beginning,
    std::string ending,
    Report::Format format,
    std::ostream &os) const {
    int ret =
        sqlite3_bind_text(
            this->stmt_report_list,
            1,
            beginning.c_str(),
            beginning.size(),
            0
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_text(
            this->stmt_report_list,
            2,
            ending.c_str(),
            ending.size(),
            0
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ReportList report;
    report(this->stmt_report_list, format, os);
}

void TBCLI::Connector::report_lookup(
    std::string path,
    Report::Format format,
    std::ostream &os) const {
    int ret =
        sqlite3_bind_text(
            this->stmt_report_lookup,
            1,
            path.c_str(),
            path.size(),
            0
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ReportLookup report;
    report(this->stmt_report_lookup, format, os);
}

void TBCLI::Connector::report_transaction(
    std::string beginning,
    std::string ending,
    Report::Format format,
    std::ostream &os) const {
    int ret =
        sqlite3_bind_text(
            this->stmt_report_transaction,
            1,
            beginning.c_str(),
            beginning.size(),
            0
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ret =
        sqlite3_bind_text(
            this->stmt_report_transaction,
            2,
            ending.c_str(),
            ending.size(),
            0
        );
    if (ret) {
        this->print_err();
        throw Err::STMT_BIND;
    }
    ReportTransaction report;
    report(this->stmt_report_transaction, format, os);
}
