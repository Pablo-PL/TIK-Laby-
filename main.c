
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

unsigned char *tablica = NULL;
size_t ilosc = 0;
size_t rozmiar = 4;
size_t licznik[256] = {0};
double H = 0.0;

int odczyt_zapis(const char *nazwa_pliku)
{
    FILE *plik;
    int bajt;

    plik = fopen(nazwa_pliku, "rb");

    if (plik == NULL)
    {
        fprintf(stderr, "Nie mozna otworzyc pliku!\n");
        return 1;
    }

    tablica = malloc(rozmiar * sizeof(unsigned char));

    if (tablica == NULL)
    {
        fprintf(stderr, "Blad alokacji pamieci!\n");
        fclose(plik);
        return 1;
    }

    while ((bajt = fgetc(plik)) != EOF)
    {
        if (ilosc == rozmiar)
        {
            if (rozmiar > SIZE_MAX / 2)
            {
                fprintf(stderr, "Przekroczono limit pamieci!\n");
                free(tablica);
                tablica = NULL;
                fclose(plik);
                return 1;
            }

            rozmiar *= 2;

            unsigned char *temp = realloc(tablica, rozmiar);

            if (temp == NULL)
            {
                fprintf(stderr, "Blad powiekszania pamieci!\n");
                free(tablica);
                tablica = NULL;
                fclose(plik);
                return 1;
            }

            tablica = temp;
        }

        tablica[ilosc] = (unsigned char)bajt;
        licznik[(unsigned char)bajt]++;
        ilosc++;
    }

    if (ferror(plik))
    {
        fprintf(stderr, "Blad odczytu pliku!\n");
        free(tablica);
        tablica = NULL;
        fclose(plik);
        return 1;
    }

    if (fclose(plik) != 0)
    {
        fprintf(stderr, "Blad zamykania pliku!\n");
        free(tablica);
        tablica = NULL;
        return 1;
    }

    return 0;
}

void z1()
{
    printf("\n");
    printf("========================================\n");
    printf("              ZADANIE 1.\n");
    printf("========================================\n");

    printf("Rozmiar pliku: %zu bajtow\n", ilosc);
}

void z2()
{
    printf("\n");
    printf("========================================\n");
    printf("              ZADANIE 2.\n");
    printf("========================================\n");

    int rozne = 0;

    for (int i = 0; i < 256; i++)
    {
        if (licznik[i] > 0)
        {
            rozne++;
        }
    }

    printf("Liczba roznych wartosci bajtow: %d\n", rozne);
}

void z3()
{
    printf("\n");
    printf("========================================\n");
    printf("              ZADANIE 3.\n");
    printf("========================================\n");

    H = 0.0;

    if (ilosc > 0)
    {
        for (int i = 0; i < 256; i++)
        {
            if (licznik[i] > 0)
            {
                double p = (double)licznik[i] / (double)ilosc;
                H -= p * log2(p);
            }
        }
    }

    printf("Entropia H: %.6f bitow/symbol\n", H);
}

void z4()
{
    printf("\n");
    printf("========================================\n");
    printf("              ZADANIE 4.\n");
    printf("========================================\n");

    double minimalny_rozmiar = ((double)ilosc * H) / 8.0;

    printf("Teoretyczny minimalny rozmiar po kompresji: %.6f bajtow\n",
           minimalny_rozmiar);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uzycie: entropy <plik>\n");
        return 1;
    }

    if (odczyt_zapis(argv[1]) != 0)
    {
        return 1;
    }

    z1();
    z2();
    z3();
    z4();

    free(tablica);
    tablica = NULL;

    return 0;
}
