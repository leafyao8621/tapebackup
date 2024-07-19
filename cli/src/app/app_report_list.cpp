#include "app.h"

void TBCLI::App::report_list(
    std::string beginning,
    std::string ending,
    Report::Format format,
    std::ostream &os) {
    this->conn.report_list(beginning, ending, format, os);
}
