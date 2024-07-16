#include "app.h"

TBCLI::App::App() {
    this->block_size_hmac = 65536;
    this->block_size_write = 65536;
    this->block_size_read = 65536;
    this->block_size_archive = 65536;
}
