# TIiK – Laboratorium L1
## Wprowadzenie. Bitowe wejście/wyjście i entropia pliku

## Opis projektu

Projekt realizuje zadanie Z1 z laboratorium Teorii Informacji i Kodowania.
Składa się z programu `entropy`, który analizuje wskazany plik bajt po bajcie,
oraz modułu `bitio`, który umożliwia zapis i odczyt pojedynczych bitów.

Program napisano w języku C, zgodnie ze standardem C11.

## 1. Część A – program entropy

Uruchomienie programu:

```bash
entropy <plik>
```

Windows / PowerShell:

```powershell
.\entropy.exe dane.txt
```

Program odczytuje plik w trybie binarnym (`rb`) i wypisuje:
1. rozmiar pliku w bajtach,
2. liczbę różnych wartości bajtów występujących w pliku,
3. entropię Shannona `H` w bitach na symbol,
4. teoretyczny minimalny rozmiar po kompresji w bajtach.

### Wzory

Prawdopodobieństwo wystąpienia bajtu `i`:

`p_i = n_i / n`

Entropia Shannona:

`H = -sum(p_i * log2(p_i))`

Teoretyczny minimalny rozmiar po kompresji:

`S_min = n * H / 8`

Gdzie:
- `n` – liczba bajtów w pliku,
- `n_i` – liczba wystąpień bajtu `i`,
- `p_i` – prawdopodobieństwo wystąpienia bajtu `i`,
- `H` – entropia w bitach na symbol,
- `S_min` – teoretyczny minimalny rozmiar w bajtach.

Dla pustego pliku program wyświetla rozmiar `0`, liczbę różnych bajtów `0`,
entropię `0` oraz teoretyczny minimalny rozmiar `0`. Nie wykonuje dzielenia
przez zero.

## 2. Część B – moduł bitio

Moduł składa się z plików `bitio.h` (deklaracje) i `bitio.c` (implementacja).

### Zapis bitów
- `bw_init()` – inicjalizuje zapis do pliku,
- `bw_write_bit()` – zapisuje jeden bit,
- `bw_write_bits()` – zapisuje wskazaną liczbę najmłodszych bitów wartości, w kolejności MSB first,
- `bw_flush()` – dopełnia ostatni niepełny bajt zerami i zapisuje go do pliku.

### Odczyt bitów
- `br_init()` – inicjalizuje odczyt z pliku,
- `br_read_bit()` – odczytuje jeden bit; zwraca `0`, `1` albo `-1` przy końcu pliku,
- `br_read_bits()` – odczytuje wskazaną liczbę bitów; zwraca `0` przy powodzeniu lub `-1` przy końcu pliku.

Bity są zapisywane i odczytywane od najstarszego bitu bajtu (MSB first).

### Ciąg kontrolny

Wywołania:

```c
bw_write_bits(w, 0x5, 3);
bw_write_bits(w, 0x1F, 5);
bw_write_bits(w, 0x1, 2);
bw_flush(w);
```

powinny utworzyć plik o długości 2 bajtów i zawartości:

```text
BF 40
```

Ostatni niepełny bajt jest uzupełniany zerami.

## 3. Część C – testy

Program testowy modułu `bitio` zapisuje ciąg kontrolny, odczytuje zapisane
wartości i porównuje je z wartościami oczekiwanymi.

Program `entropy` uruchomiono dla sześciu przypadków wymaganych w instrukcji:
pustego pliku, 1000 powtórzeń jednego bajtu, tekstu, losowego pliku 1 MiB,
archiwum ZIP oraz pliku wykonywalnego.

### Wyniki pomiarów

Wyniki uzyskane w środowisku Windows, dla plików wygenerowanych przez
`tests/run.ps1`:

| Plik testowy | Rozmiar [B] | Różne bajty | Entropia [bit/symbol] | `n * H / 8` [B] |
|---|---:|---:|---:|---:|
| Pusty plik | 0 | 0 | 0,000000 | 0,000000 |
| 1000 identycznych bajtów | 1 000 | 1 | 0,000000 | 0,000000 |
| Tekst testowy | 156 | 27 | 4,353715 | 84,897433 |
| Losowy plik 1 MiB | 1 048 576 | 256 | 7,999836 | 1 048 554,448073 |
| Archiwum ZIP | 233 | 107 | 5,729002 | 166,857193 |
| Plik wykonywalny (`entropy.exe`) | 60 826 | 256 | 4,837547 | 36 781,081705 |

### Interpretacja wyników

1. **Pusty plik:** nie zawiera symboli, dlatego program zwraca rozmiar 0,
   liczbę różnych bajtów 0 i entropię 0. Nie występuje dzielenie przez zero.

2. **1000 identycznych bajtów:** występuje tylko jedna wartość bajtu.
   Kolejny symbol jest całkowicie przewidywalny, więc entropia wynosi 0.

3. **Tekst testowy:** entropia wynosi około 4,35 bit/symbol. Jest niższa
   niż 8 bitów/symbol, ponieważ znaki w tekście nie występują z jednakową
   częstością i pojawiają się powtarzalne wzorce.

4. **Losowy plik 1 MiB:** entropia wynosi około 8 bitów/symbol, a w pliku
   wystąpiły wszystkie 256 wartości bajtu. Jest to wynik zgodny z oczekiwaniem
   dla danych o rozkładzie zbliżonym do jednostajnego.

5. **Archiwum ZIP:** uzyskano 5,73 bitów/symbol. Jest to małe archiwum
   (233 bajty), utworzone z krótkiego tekstu. Nagłówki i niewielki rozmiar
   próbki mają istotny wpływ na zmierzony rozkład bajtów. Wynik nie musi
   być bliski 8 bitom/symbol dla tak małego archiwum.

6. **Plik wykonywalny:** entropia wynosi około 4,84 bitów/symbol.
   Plik zawiera strukturę i powtarzalne elementy, dlatego rozkład wartości
   bajtów nie jest jednostajny.

Wartość `n * H / 8` jest teoretycznym oszacowaniem dla kodowania
bezpamięciowego, w którym każdy bajt jest kodowany niezależnie. Nie jest
gwarancją rozmiaru konkretnego pliku po zastosowaniu rzeczywistego
algorytmu kompresji. W szczególności nie uwzględnia zależności pomiędzy
kolejnymi bajtami ani narzutu formatu kompresji.

## 4. Kompilacja

Wymagane flagi:

`-std=c11 -Wall -Wextra -O2`

GCC / MinGW-w64:

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c bitio.c -o entropy -lm
```

Windows:

```powershell
gcc -std=c11 -Wall -Wextra -O2 main.c bitio.c -o entropy.exe -lm
```

Jeżeli projekt jest budowany przez Makefile:

```bash
make
```

## 5. Uruchamianie

Linux:

```bash
./entropy dane.txt
```

Windows / PowerShell:

```powershell
.\entropy.exe dane.txt
```

Program wymaga dokładnie jednego argumentu – ścieżki do analizowanego pliku.
Niepoprawna liczba argumentów lub błąd odczytu powoduje komunikat na `stderr`
i zakończenie programu niezerowym kodem.

## 6. Uruchamianie testów

Test modułu bitowego:

```powershell
gcc -std=c11 -Wall -Wextra -O2 -I. tests/test_bitio.c bitio.c -o tests/test_bitio.exe
.\tests\test_bitio.exe
```

Testy programu `entropy`:

```powershell
powershell -ExecutionPolicy Bypass -File tests\run.ps1
```

Skrypt tworzy pliki testowe i zapisuje wyniki do
`tests/wyniki_pomiarow.txt`.

## 7. Kontrola pamięci

Na Linuxie można użyć Valgrind:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./entropy dane.txt
```

Na Windowsie można użyć Dr. Memory, zgodnie z konfiguracją środowiska:

```powershell
drmemory -- entropy.exe dane.txt
```

## 8. Wymagania techniczne

- Język C, standard C11.
- Kompilacja z flagami `-std=c11 -Wall -Wextra -O2`.
- Brak ostrzeżeń kompilatora.
- Pliki otwierane w trybie binarnym (`rb` do odczytu, `wb` do zapisu).
- Obsługa błędów wejścia/wyjścia i błędnych argumentów.
- Komunikaty o błędach kierowane na `stderr`.
- Niezerowy kod wyjścia w przypadku błędu.
- Brak wycieków pamięci.

## 9. Struktura projektu

```text
projekt/
├── stderr/
├── tests/
│   ├── test_bitio.c
│   ├── run.ps1
│   └── pliki testowe generowane przez skrypt
├── bitio.c
├── bitio.h
├── main.c
├── Makefile
├── build.bat
└── README.md
```
