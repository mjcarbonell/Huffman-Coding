#ifndef __IO_H__
#define __IO_H__

#include "code.h"
#include "defines.h"
#include "io.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

extern uint64_t bytes_read;
extern uint64_t bytes_written;

int read_bytes(int infile, uint8_t *buf, int nbytes);

int write_bytes(int outfile, uint8_t *buf, int nbytes);

bool read_bit(int infile, uint8_t *bit);

void write_code(int outfile, Code *c);

void flush_codes(int outfile);

#endif
