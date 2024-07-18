#include "app.h"

void TBCLI::App::report_daily(
    std::string beginning,
    std::string ending,
    Report::Format format,
    std::ostream &os) {
    this->conn.report_daily(beginning, ending, format, os);
}
