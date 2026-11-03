# Analiza pliku binarnego

## Opis

Program w jezyku C11 odczytuje plik bajt po bajcie w trybie binarnym,
zlicza wystapienia wszystkich mozliwych wartosci bajtu (0-255),
oblicza entropie Shannona oraz szacuje teoretyczny minimalny rozmiar
po kompresji.

## Funkcjonalnosci

1. Obliczanie rozmiaru pliku w bajtach.
2. Zliczanie wystapien kazdego bajtu.
3. Obliczanie entropii Shannona w bitach na symbol.
4. Obliczanie teoretycznego minimalnego rozmiaru po kompresji:

   `S_min = n * H / 8`

## Wymagania

- Kompilator GCC obslugujacy C11.
- Make (opcjonalnie, do korzystania z Makefile).
- Standardowa biblioteka C oraz biblioteka matematyczna dostarczana
  z kompilatorem (`log2` z `math.h`).

## Kompilacja

Linux / GCC:

```bash
make
```

Kompilacja bez Make:

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c -o program -lm
```

Windows / MinGW-w64:

Uruchom `build.bat` albo wykonaj:

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c -o program.exe -lm
```

## Uruchomienie

Domyslny plik `dane.txt`:

```bash
./program
```

Wskazanie pliku:

```bash
./program dane.bin
```

W Windows:

```powershell
.\program.exe dane.bin
```

## Testy

Linux / Git Bash:

```bash
make test
```

Skrypt sprawdza pusty plik, tekst o znanym rozkladzie bajtow,
plik binarny zawierajacy m.in. bajty `0x00` i `0xFF`, brak pliku
oraz bledna liczbe argumentow.

## Zarzadzanie pamiecia

Pamiec na dane jest przydzielana dynamicznie przez `malloc` i
powiekszana przez `realloc`. W przypadku bledow alokacji program
zwalnia zaalokowana pamiec. Po wykonaniu obliczen pamiec jest
zwalniana przez `free`.

## Wyniki pomiarow

Nie wpisano wynikow pomiarow dla pliku uzytkownika, poniewaz zalezy
to od badanego pliku i srodowiska uruchomieniowego.

Do pomiarow czasu na Linuxie mozna uzyc:

```bash
/usr/bin/time -f "Czas: %e s, pamiec maks.: %M KB" ./program dane.bin
```

Przykladowy test kontrolny:

Plik `AAAAABBBCC` ma 10 bajtow. Czestotliwosci symboli wynosza:
- A: 5
- B: 3
- C: 2

Entropia dla tego rozkladu wynosi okolo `1.485475` bitow/symbol,
a teoretyczny minimalny rozmiar wedlug wzoru `n * H / 8` wynosi
okolo `1.856844` bajta.

## Uwagi

- Plik jest otwierany w trybie `rb`, co zapobiega konwersji koncow linii
  i traktowaniu bajtu `0x1A` jako konca pliku w systemie Windows.
- Dla pustego pliku entropia i szacowany rozmiar na podstawie entropii
  sa oznaczane jako nieokreslone.
- Wynik zadania 4 jest teoretycznym oszacowaniem opartym na entropii
  empirycznej. Nie jest to gwarancja rozmiaru konkretnego pliku
  po zastosowaniu rzeczywistego algorytmu kompresji.
