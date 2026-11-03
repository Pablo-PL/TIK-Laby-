
#include "bitio.h"

/* ============================= */
/*          ZAPIS BITOW           */
/* ============================= */

void bw_init(BitWriter *w, FILE *f)
{
    w->f = f;
    w->buf = 0;
    w->nbits = 0;
}

void bw_write_bit(BitWriter *w, int bit)
{
    w->buf = (uint8_t)((w->buf << 1) | (bit & 1));
    w->nbits++;

    if (w->nbits == 8)
    {
        fputc(w->buf, w->f);
        w->buf = 0;
        w->nbits = 0;
    }
}

void bw_write_bits(BitWriter *w, uint32_t value, int n)
{
    for (int i = n - 1; i >= 0; i--)
    {
        bw_write_bit(w, (value >> i) & 1);
    }
}

void bw_flush(BitWriter *w)
{
    if (w->nbits > 0)
    {
        w->buf <<= (8 - w->nbits);
        fputc(w->buf, w->f);

        w->buf = 0;
        w->nbits = 0;
    }
}

/* ============================= */
/*          ODCZYT BITOW          */
/* ============================= */

void br_init(BitReader *r, FILE *f)
{
    r->f = f;
    r->buf = 0;
    r->nbits = 0;
}

int br_read_bit(BitReader *r)
{
    if (r->nbits == 0)
    {
        int bajt = fgetc(r->f);

        if (bajt == EOF)
            return -1;

        r->buf = (uint8_t)bajt;
        r->nbits = 8;
    }

    int bit = (r->buf >> 7) & 1;

    r->buf <<= 1;
    r->nbits--;

    return bit;
}

int br_read_bits(BitReader *r, int n, uint32_t *out)
{
    if (n < 0 || n > 32)
        return -1;

    *out = 0;

    for (int i = 0; i < n; i++)
    {
        int bit = br_read_bit(r);

        if (bit == -1)
            return -1;

        *out = (*out << 1) | (uint32_t)bit;
    }

    return 0;
}
