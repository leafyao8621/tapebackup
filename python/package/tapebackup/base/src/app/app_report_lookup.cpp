#include "app.h"

void TBCLI::App::report_lookup(
    std::string path,
    Report::Format format,
    std::ostream &os) {
    this->conn.report_lookup(path, format, os);
}
