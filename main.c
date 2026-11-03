#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

unsigned char *tablica = NULL;
size_t ilosc = 0;
size_t rozmiar = 4;
size_t licznik[256] = {0};
double entropia = 0.0;

int odczyt_zapis(const char *nazwa_pliku)
{
    FILE *plik;
    int bajt;

    plik = fopen(nazwa_pliku, "rb");

    if (plik == NULL)
    {
        fprintf(stderr, "Blad: nie mozna otworzyc pliku: %s\n", nazwa_pliku);
        return 1;
    }

    tablica = malloc(rozmiar * sizeof(unsigned char));

    if (tablica == NULL)
    {
        fprintf(stderr, "Blad: nie udalo sie przydzielic pamieci.\n");
        fclose(plik);
        return 1;
    }

    while ((bajt = fgetc(plik)) != EOF)
    {
        if (ilosc == rozmiar)
        {
            if (rozmiar > SIZE_MAX / 2)
            {
                fprintf(stderr, "Blad: przekroczono maksymalny rozmiar tablicy.\n");
                free(tablica);
                tablica = NULL;
                fclose(plik);
                return 1;
            }

            rozmiar *= 2;

            unsigned char *temp = realloc(tablica, rozmiar);

            if (temp == NULL)
            {
                fprintf(stderr, "Blad: nie udalo sie powiekszyc tablicy.\n");
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
        fprintf(stderr, "Blad: wystapil blad podczas odczytu pliku.\n");
        free(tablica);
        tablica = NULL;
        fclose(plik);
        return 1;
    }

    if (fclose(plik) != 0)
    {
        fprintf(stderr, "Blad: nie udalo sie poprawnie zamknac pliku.\n");
        free(tablica);
        tablica = NULL;
        return 1;
    }

    return 0;
}

void z1(void)
{
    printf("\n");
    printf("========================================\n");
    printf("              ZADANIE 1.\n");
    printf("========================================\n");

    printf("Rozmiar pliku: %zu bajtow\n", ilosc);
}

void z2(void)
{
    printf("\n");
    printf("========================================\n");
    printf("              ZADANIE 2.\n");
    printf("========================================\n");

    printf("Wystapienia poszczegolnych bajtow:\n");

    for (int i = 0; i < 256; i++)
    {
        if (licznik[i] > 0)
        {
            printf("0x%02X : %zu\n", i, licznik[i]);
        }
    }
}

void z3(void)
{
    printf("\n");
    printf("========================================\n");
    printf("              ZADANIE 3.\n");
    printf("========================================\n");

    entropia = 0.0;

    if (ilosc == 0)
    {
        printf("Plik jest pusty. Entropia nie jest okreslona.\n");
        return;
    }

    for (int i = 0; i < 256; i++)
    {
        if (licznik[i] > 0)
        {
            double p = (double)licznik[i] / (double)ilosc;
            entropia -= p * log2(p);
        }
    }

    printf("Entropia H: %.6f bitow/symbol\n", entropia);
}

void z4(void)
{
    printf("\n");
    printf("========================================\n");
    printf("              ZADANIE 4.\n");
    printf("========================================\n");

    if (ilosc == 0)
    {
        printf("Plik jest pusty. Nie mozna wyznaczyc rozmiaru na podstawie entropii.\n");
        return;
    }

    {
        double minimalny_rozmiar = ((double)ilosc * entropia) / 8.0;

        printf("Teoretyczny minimalny rozmiar po kompresji:\n");
        printf("n * H / 8 = %zu * %.6f / 8 = %.6f bajtow\n\n", ilosc, entropia, minimalny_rozmiar);
    }
}

int main(int argc, char *argv[])
{
    const char *nazwa_pliku = "dane.txt";

    if (argc > 2)
    {
        fprintf(stderr, "Uzycie: %s [nazwa_pliku]\n", argv[0]);
        return 1;
    }

    if (argc == 2)
    {
        nazwa_pliku = argv[1];
    }

    if (odczyt_zapis(nazwa_pliku) != 0)
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
