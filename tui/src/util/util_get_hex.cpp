#include "util.h"

void TBTUI::Util::get_hex(char *buf, char *out) {
    char *iter = buf;
    char *iter_out = out;
    for (size_t i = 0; i < 64; ++i, ++iter, iter_out += 2) {
        snprintf(iter_out, 3, "%02hhX", *iter);
    }
}
