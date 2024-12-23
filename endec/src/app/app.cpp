#include "app.h"

TBENDEC::App::App() {
    this->block_size_hmac = 65536;
    this->block_size_write = 65536;
    this->block_size_read = 65536;
    this->block_size_archive = 65536;
}

void TBENDEC::App::set_block_size_archive(size_t size) {
    this->block_size_archive = size;
}

void TBENDEC::App::set_block_size_hmac(size_t size) {
    this->block_size_hmac = size;
}

void TBENDEC::App::set_block_size_read(size_t size) {
    this->block_size_read = size;
}

void TBENDEC::App::set_block_size_write(size_t size) {
    this->block_size_write = size;
}
