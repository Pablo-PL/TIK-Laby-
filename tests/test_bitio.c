
#include <stdio.h>
#include <stdint.h>
#include "bitio.h"

int main()
{
    FILE *plik;
    BitWriter w;

    printf("TEST ZAPISU BITOW\n");

    plik = fopen("test.bin", "wb");

    if (plik == NULL)
    {
        printf("Blad otwarcia pliku!\n");
        return 1;
    }

    bw_init(&w, plik);

    bw_write_bits(&w, 0x5, 3);
    bw_write_bits(&w, 0x1F, 5);
    bw_write_bits(&w, 0x1, 2);

    bw_flush(&w);

    if (ferror(plik))
    {
        printf("Blad zapisu!\n");
        fclose(plik);
        return 1;
    }

    fclose(plik);

    printf("Zapis zakonczony.\n");

    printf("\nTEST ODCZYTU BITOW\n");

    plik = fopen("test.bin", "rb");

    if (plik == NULL)
    {
        printf("Blad otwarcia pliku!\n");
        return 1;
    }

    BitReader r;
    uint32_t a, b, c;

    br_init(&r, plik);

    if (br_read_bits(&r, 3, &a) != 0 ||
        br_read_bits(&r, 5, &b) != 0 ||
        br_read_bits(&r, 2, &c) != 0)
    {
        printf("Blad odczytu bitow!\n");
        fclose(plik);
        return 1;
    }

    printf("Odczytane wartosci:\n");
    printf("a = 0x%X\n", (unsigned int)a);
    printf("b = 0x%X\n", (unsigned int)b);
    printf("c = 0x%X\n", (unsigned int)c);

    fclose(plik);

    if (a == 0x5 && b == 0x1F && c == 0x1)
        printf("\nTEST ZALICZONY!\n");
    else
        printf("\nTEST NIEZALICZONY!\n");

    return 0;
}
