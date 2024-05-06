#include "util.h"

bool TBCLI::Util::check_dev(FILE *dev) {
    fseek(dev, 0, 0);
    unsigned short signature = 0;
    fread(&signature, 2, 1, dev);
    return signature == 0x104a;
}

bool TBCLI::Util::check_dev_write_protection(FILE *dev) {
    fseek(dev, 2, 0);
    unsigned short signature = 0;
    fread(&signature, 2, 1, dev);
    return signature == 0x90b0;
}

void TBCLI::Util::init_dev(FILE *dev, bool write_protection) {
    fseek(dev, 0, 0);
    unsigned short signature = 0x104a;
    fwrite(&signature, 2, 1, dev);
    signature = write_protection ? 0x90b0 : 0xb090;
    fwrite(&signature, 2, 1, dev);
}

void TBCLI::Util::set_dev_write_protection(FILE *dev) {
    fseek(dev, 2, 0);
    unsigned short signature = 0x90b0;
    fwrite(&signature, 2, 1, dev);
}
