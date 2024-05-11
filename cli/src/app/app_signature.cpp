#include <iostream>
#include <sstream>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include "app.h"

TBCLI::App::Signature::Signature() {
    this->rnd_src = 0;
    this->signature_file = 0;
}

TBCLI::App::Signature::~Signature() {
    if (this->rnd_src) {
        close(this->rnd_src);
    }
    if (this->signature_file) {
        close(this->signature_file);
    }
}

void TBCLI::App::Signature::init() {
    std::ostringstream oss;
    oss << getenv("HOME") << "/.signature";
    this->signature_file = open(oss.str().c_str(), O_RDWR | O_APPEND);
    this->rnd_src = open("/dev/urandom", O_RDONLY);
    char buf[64];
    ssize_t bytes_read = 0;
    for (; (bytes_read = read(this->signature_file, buf, 64)) == 64;) {
        this->valid_list.push_back(std::vector<char>(buf, buf + 64));
    }
}

bool TBCLI::App::Signature::check(char *signature) const {
    for (const auto &i : this->valid_list) {
        if (!memcmp(i.data(), signature, 64)) {
            return true;
        }
    }
    return false;
}

void TBCLI::App::Signature::generate(char *signature) const {
    ssize_t bytes_read = read(this->rnd_src, signature, 64);
    if (!bytes_read || bytes_read == -1) {
        throw GENERATOR_FAIL;
    }
}

void TBCLI::App::Signature::commit(char *signature) const {
    ssize_t bytes_written = write(this->signature_file, signature, 64);
    if (bytes_written != 64) {
        throw WRITE_FAIL;
    }
}
