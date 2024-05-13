#include "util.h"

TBCLI::Util::HMAC::HMAC() {
    this->evp_mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    this->evp_mac_ctx = EVP_MAC_CTX_new(evp_mac);
}

TBCLI::Util::HMAC::~HMAC() {
    EVP_MAC_CTX_free(this->evp_mac_ctx);
    EVP_MAC_free(this->evp_mac);
}
