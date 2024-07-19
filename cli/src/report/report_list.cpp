#include "report.h"

void TBCLI::ReportList::report_text(sqlite3_stmt *stmt, std::ostream &os) {
    int ret = 0;
    for (;;) {
        ret = sqlite3_step(stmt);
        if (ret == SQLITE_DONE) {
            break;
        }
        if (ret != SQLITE_ROW) {
            throw Err::STMT_EXECUTION;
        }
        os << "Signature: " << sqlite3_column_text(stmt, 0) << std::endl <<
            "Write protection: " <<
            sqlite3_column_int64(stmt, 1) << std::endl <<
            "HMAC key: " <<
            sqlite3_column_text(stmt, 2) << std::endl <<
            "File name: " <<
            sqlite3_column_text(stmt, 3) << std::endl <<
            "HMAC value: " <<
            sqlite3_column_text(stmt, 4) << std::endl <<
            "Start time: " <<
            sqlite3_column_text(stmt, 5) << std::endl <<
            "Completion time: " <<
            sqlite3_column_text(stmt, 6) << std::endl <<
            "Reported size: " <<
            sqlite3_column_int64(stmt, 7) << std::endl <<
            "Written size: " <<
            sqlite3_column_int64(stmt, 8) << std::endl;
    }
    ret = sqlite3_reset(stmt);
    if (ret) {
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::ReportList::report_csv(sqlite3_stmt *stmt, std::ostream &os) {
    int ret = 0;
    os <<
        "\"Signature\"," <<
        "\"Write Protection\"," <<
        "\"HMAC Key\"," <<
        "\"File Name\"," <<
        "\"HMAC Value\"," <<
        "\"Start Time\"," <<
        "\"Completion Time\"," <<
        "\"Reported Size\"," <<
        "\"Written Size\"" << std::endl;
    for (;;) {
        ret = sqlite3_step(stmt);
        if (ret == SQLITE_DONE) {
            break;
        }
        if (ret != SQLITE_ROW) {
            throw Err::STMT_EXECUTION;
        }
        os << '"' << sqlite3_column_text(stmt, 0) << '"' << ',' <<
            sqlite3_column_int64(stmt, 1) << ',' <<
            '"' << sqlite3_column_text(stmt, 2) << '"' << ',' <<
            '"' << sqlite3_column_text(stmt, 3) << '"' << ',' <<
            '"' << sqlite3_column_text(stmt, 4) << '"' << ',' <<
            '"' << sqlite3_column_text(stmt, 5) << '"' << ',' <<
            '"' << sqlite3_column_text(stmt, 6) << '"' << ',' <<
            sqlite3_column_int64(stmt, 7) << ',' <<
            sqlite3_column_int64(stmt, 8) << std::endl;
    }
    ret = sqlite3_reset(stmt);
    if (ret) {
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::ReportList::operator()(
            sqlite3_stmt *stmt, Format format, std::ostream &out) {
    switch (format) {
    case Format::TEXT:
        this->report_text(stmt, out);
        break;
    case Format::CSV:
        this->report_csv(stmt, out);
        break;
    }
}
