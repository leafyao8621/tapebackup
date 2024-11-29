#include "util.h"

void TBENDEC::Util::get_hex(char *buf, char *out) {
    char *iter = buf;
    char *iter_out = out;
    for (size_t i = 0; i < 64; ++i, ++iter, iter_out += 2) {
        snprintf(iter_out, 3, "%02hhX", *iter);
    }
}

void TBENDEC::Util::parse_hex(char *buf, char *out) {
    char *iter = buf;
    char *iter_out = out;
    for (size_t i = 0; i < 64; ++i, iter += 2, ++iter_out) {
        *iter_out =
            (
                (
                    *iter >= 'A' ?
                    *iter - 'A' + 0xa:
                    *iter - '0'
                ) << 4
            ) |
            (
                iter[1] >= 'A' ?
                iter[1] - 'A' + 0xa:
                iter[1] - '0'
            );
    }
}
