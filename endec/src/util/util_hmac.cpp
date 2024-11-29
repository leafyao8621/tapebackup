#include <iostream>

#include <fcntl.h>
#include <unistd.h>

#include "util.h"

TBENDEC::Util::HMAC::HMAC(size_t block_size) {
    this->block_size = block_size;
    this->buf = new char[block_size];
    this->evp_mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    if (!this->evp_mac) {
        throw Err::CREATION;
    }
    this->evp_mac_ctx = EVP_MAC_CTX_new(evp_mac);
    if (!this->evp_mac_ctx) {
        throw Err::CREATION;
    }
    this->param[0] =
        OSSL_PARAM_construct_utf8_string("digest", (char*)"SHA2-512", 0);
    this->param[1] = OSSL_PARAM_construct_end();
    this->fd = -1;
}

TBENDEC::Util::HMAC::~HMAC() {
    delete[] this->buf;
    EVP_MAC_CTX_free(this->evp_mac_ctx);
    EVP_MAC_free(this->evp_mac);
    if (this->fd != -1) {
        close(this->fd);
    }
}

void TBENDEC::Util::HMAC::operator()(
    char *path,
    char *key,
    char *md,
    bool verbose) {
    if (
        !EVP_MAC_init(
            this->evp_mac_ctx, (const unsigned char*)key, 64, this->param)) {
        throw Err::INITIALIZATION;
    }
    this->fd = open(path, O_RDONLY);
    ssize_t bytes_read = 0;
    for (
        size_t i = 0;
        (bytes_read = read(this->fd, this->buf, this->block_size));
        ++i) {
        if (verbose) {
            std::cout << "Calculating block " << i << std::endl;
        }
        if (
            !EVP_MAC_update(
                this->evp_mac_ctx,
                (const unsigned char*)this->buf,
                bytes_read
            )) {
            throw Err::UPDATE;
        }
    }
    size_t final_l = 0;
    if (!EVP_MAC_final(this->evp_mac_ctx, (unsigned char*)md, &final_l, 64)) {
        throw Err::FINALIZATION;
    }
}
