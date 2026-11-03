
#ifndef BITIO_H
#define BITIO_H

#include <stdio.h>
#include <stdint.h>

typedef struct {
    FILE *f;
    uint8_t buf;
    int nbits;
} BitWriter;

typedef struct {
    FILE *f;
    uint8_t buf;
    int nbits;
} BitReader;

/* Zapis bitow */
void bw_init(BitWriter *w, FILE *f);
void bw_write_bit(BitWriter *w, int bit);
void bw_write_bits(BitWriter *w, uint32_t value, int n);
void bw_flush(BitWriter *w);

/* Odczyt bitow */
void br_init(BitReader *r, FILE *f);
int br_read_bit(BitReader *r);
int br_read_bits(BitReader *r, int n, uint32_t *out);

#endif
