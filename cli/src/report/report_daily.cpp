#include "report.h"

void TBCLI::ReportDaily::report_text(sqlite3_stmt *stmt, std::ostream &os) {
    int ret = 0;
    for (;;) {
        ret = sqlite3_step(stmt);
        if (ret == SQLITE_DONE) {
            break;
        }
        if (ret != SQLITE_ROW) {
            throw Err::STMT_EXECUTION;
        }
        os << "Date: " << sqlite3_column_text(stmt, 0) << std::endl <<
            "Reported size: " << sqlite3_column_int64(stmt, 1) << std::endl <<
            "Written size: " << sqlite3_column_int64(stmt, 2) << std::endl;
    }
    ret = sqlite3_reset(stmt);
    if (ret) {
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::ReportDaily::report_csv(sqlite3_stmt *stmt, std::ostream &os) {
    int ret = 0;
    os << "\"Date\",\"Reported Size\",\"Written Size\"" << std::endl;
    for (;;) {
        ret = sqlite3_step(stmt);
        if (ret == SQLITE_DONE) {
            break;
        }
        if (ret != SQLITE_ROW) {
            throw Err::STMT_EXECUTION;
        }
        os << sqlite3_column_text(stmt, 0) << ',' <<
            sqlite3_column_int64(stmt, 1) << ',' <<
            sqlite3_column_int64(stmt, 2) << std::endl;
    }
    ret = sqlite3_reset(stmt);
    if (ret) {
        throw Err::STMT_EXECUTION;
    }
}

void TBCLI::ReportDaily::operator()(
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
