#ifndef REPORT_REPORT_H_
#define REPORT_REPORT_H_

#include <iostream>

#include <sqlite3.h>

namespace TBCLI {
    class Report {
        virtual void report_text(sqlite3_stmt *stmt, std::ostream &os) = 0;
        virtual void report_csv(sqlite3_stmt *stmt, std::ostream &os) = 0;
    public:
        enum Format {
            TEXT,
            CSV
        };
        enum Err {
            STMT_EXECUTION
        };
        virtual void operator()(
            sqlite3_stmt *stmt, Format format, std::ostream &out) = 0;
    };
    class ReportDaily : Report {
    public:
        void report_text(sqlite3_stmt *stmt, std::ostream &os);
        void report_csv(sqlite3_stmt *stmt, std::ostream &os);
        void operator()(
            sqlite3_stmt *stmt, Format format, std::ostream &out);
    };
    class ReportList : Report {
    public:
        void report_text(sqlite3_stmt *stmt, std::ostream &os);
        void report_csv(sqlite3_stmt *stmt, std::ostream &os);
        void operator()(
            sqlite3_stmt *stmt, Format format, std::ostream &out);
    };
    class ReportLookup : Report {
    public:
        void report_text(sqlite3_stmt *stmt, std::ostream &os);
        void report_csv(sqlite3_stmt *stmt, std::ostream &os);
        void operator()(
            sqlite3_stmt *stmt, Format format, std::ostream &out);
    };
    class ReportTransaction : Report {
    public:
        void report_text(sqlite3_stmt *stmt, std::ostream &os);
        void report_csv(sqlite3_stmt *stmt, std::ostream &os);
        void operator()(
            sqlite3_stmt *stmt, Format format, std::ostream &out);
    };
}

#endif
