#include "app.h"

void TBCLI::App::report_transaction(
    std::string beginning,
    std::string ending,
    Report::Format format,
    std::ostream &os) {
    this->conn.report_transaction(beginning, ending, format, os);
}
