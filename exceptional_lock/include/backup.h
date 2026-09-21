#ifndef BACKUP_H
#define BACKUP_H

#include <sodium.h>

void generate_dynamic_key(unsigned char key_out[crypto_secretbox_KEYBYTES]);

#endif
