#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>

#include "util.h"

TBCLI::Util::HMAC::HMAC() {
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
}

TBCLI::Util::HMAC::~HMAC() {
    EVP_MAC_CTX_free(this->evp_mac_ctx);
    EVP_MAC_free(this->evp_mac);
}

void TBCLI::Util::HMAC::operator()(char *file_name, char *key, char *md) const {
    if (
        !EVP_MAC_init(
            this->evp_mac_ctx, (const unsigned char*)key, 64, this->param)) {
        throw Err::INITIALIZATION;
    }
    char buf[64000];
    std::ostringstream oss_archive;
    oss_archive << getenv("HOME") << "/backup/archive/" << file_name << ".tar";
    int fd = open(oss_archive.str().c_str(), O_RDONLY);
    ssize_t bytes_read = 0;
    for (; (bytes_read = read(fd, buf, 64000));) {
        if (
            !EVP_MAC_update(
                this->evp_mac_ctx, (const unsigned char*)buf, bytes_read)) {
            close(fd);
            throw Err::UPDATE;
        }
    }
    close(fd);
    size_t final_l = 0;
    if (!EVP_MAC_final(this->evp_mac_ctx, (unsigned char*)md, &final_l, 64)) {
        throw Err::FINALIZATION;
    }
}
