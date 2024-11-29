#include <iostream>

#include <fcntl.h>
#include <unistd.h>

#include "util.h"

TBENDEC::Util::Writer::Writer(size_t block_size) {
    this->block_size = block_size;
    this->buf = new char[block_size];
    this->cipher_buf =
        new char[
            this->block_size +
            EVP_CIPHER_get_block_size(EVP_aes_256_cbc())
        ];
    this->fd_in = -1;
    this->fd_out = -1;
    this->ctx = EVP_CIPHER_CTX_new();
}

TBENDEC::Util::Writer::~Writer() {
    delete[] this->buf;
    delete[] this->cipher_buf;
    if (this->fd_in != -1) {
        close(this->fd_in);
    }
    if (this->fd_out != -1) {
        close(this->fd_out);
    }
    EVP_CIPHER_CTX_free(this->ctx);
}

size_t TBENDEC::Util::Writer::operator()(
    char *path,
    char *dev,
    char *key,
    bool verbose) {
    size_t out = 0;
    this->fd_in = open(path, O_RDONLY);
    std::cout << path << std::endl;
    if (fd_in == -1) {
        throw Err::OPEN;
    }
    this->fd_out = open(dev, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd_out == -1) {
        throw Err::OPEN;
    }
    if (
        !EVP_EncryptInit(
            this->ctx,
            EVP_aes_256_cbc(),
            (const unsigned char*)key,
            (const unsigned char*)(key + 32))) {
        throw Err::CIPHER_INIT;
    };
    ssize_t bytes_read = 0;
    int out_len = 0;
    for (
        size_t i = 0;
        (bytes_read = read(this->fd_in, this->buf, this->block_size));
        ++i) {
        if (bytes_read == -1) {
            throw Err::READ;
        }
        if (verbose) {
            std::cout << "Writing block " << i << std::endl;
        }
        if (
            !EVP_CipherUpdate(
                this->ctx,
                (unsigned char *)this->cipher_buf,
                &out_len,
                (const unsigned char*)this->buf,
                bytes_read)) {
            throw Err::CIPHER_UPDATE;
        }
        ssize_t bytes_written = write(this->fd_out, this->cipher_buf, out_len);
        if (bytes_written == -1) {
            throw Err::WRITE;
        }
        out += bytes_written;
    }
    if (
        !EVP_EncryptFinal(
            this->ctx,
            (unsigned char *)this->cipher_buf,
            &out_len)) {
        throw Err::CIPHER_FINAL;
    }
    ssize_t bytes_written = write(this->fd_out, this->cipher_buf, out_len);
    if (bytes_written == -1) {
        throw Err::WRITE;
    }
    out += bytes_written;
    return out;
}
