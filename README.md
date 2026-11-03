# TIiK – Laboratorium L1
## Bitowe wejście/wyjście i entropia pliku

### Opis projektu

Projekt realizuje zadanie Z1 z laboratorium Teorii Informacji i Kodowania.
Składa się z programu `entropy`, który analizuje wskazany plik bajt po bajcie,
oraz modułu `bitio`, który umożliwia zapis i odczyt pojedynczych bitów.

Program jest napisany w języku C w standardzie C11.

## 1. Część A – program entropy

Program uruchamia się poleceniem:

```bash
entropy <plik>
```

W systemie Windows plik wykonywalny może mieć nazwę `entropy.exe`:

```powershell
.\entropy.exe dane.txt
```

Program odczytuje plik w trybie binarnym (`rb`) i wyświetla:

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

Moduł składa się z plików:
- `bitio.h` – deklaracje struktur i funkcji,
- `bitio.c` – implementacja funkcji.

Moduł udostępnia struktury `BitWriter` i `BitReader` oraz funkcje:

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

Wymagany ciąg wywołań:

```c
bw_write_bits(w, 0x5, 3);
bw_write_bits(w, 0x1F, 5);
bw_write_bits(w, 0x1, 2);
bw_flush(w);
```

Powinien utworzyć plik o długości 2 bajtów i zawartości:

```text
BF 40
```

Ostatni niepełny bajt jest uzupełniany zerami.

## 3. Część C – testy

Program testowy modułu `bitio` zapisuje ciąg kontrolny, odczytuje zapisane wartości
i porównuje je z wartościami oczekiwanymi.

Program `entropy` należy sprawdzić na następujących danych:

| Plik testowy | Oczekiwany wynik |
|---|---|
| Pusty plik | Brak błędu, `H = 0` |
| 1000 powtórzeń tego samego bajtu | `H = 0` |
| Tekst w języku polskim, np. kod źródłowy | `H` około 4–5 bit/symbol |
| Plik losowy o rozmiarze 1 MB | `H` bliska 8 bit/symbol |
| Archiwum ZIP lub plik JPG | `H` bliska 8 bit/symbol |
| Plik wykonywalny | `H` wyraźnie mniejsza od 8 bit/symbol |

Pliki testowe należy zachować, ponieważ będą wykorzystywane w kolejnych zadaniach.

## 4. Kompilacja

Wymagane flagi:

`-std=c11 -Wall -Wextra -O2`

Przykładowa kompilacja programu (GCC / MinGW-w64):

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c bitio.c -o entropy -lm
```

W systemie Windows:

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

## 6. Testowanie

Test modułu bitowego:

```powershell
gcc -std=c11 -Wall -Wextra -O2 test_bitio.c bitio.c -o test_bitio.exe
.\test_bitio.exe
```

Oczekiwany wynik testu ciągu kontrolnego to dwa bajty `BF 40`, a odczytane
wartości powinny być równe `0x5`, `0x1F` i `0x1`.

Testy programu `entropy` należy wykonać dla wszystkich przypadków wymienionych
w części C.

## 7. Kontrola pamięci

Na Linuxie można użyć Valgrind:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./entropy dane.txt
```

Na Windowsie można użyć Dr. Memory, zgodnie z konfiguracją środowiska:

```powershell
drmemory -- entropy.exe dane.txt
```

## 8. Wyniki pomiarów

Wyniki należy uzupełnić po uruchomieniu programu na plikach testowych.
Nie należy wpisywać wartości szacunkowych zamiast rzeczywistych pomiarów.

| Plik testowy | Rozmiar [B] | Różne bajty | Entropia [bit/symbol] | `n * H / 8` [B] |
|---|---:|---:|---:|---:|
| Pusty plik | do uzupełnienia | do uzupełnienia | do uzupełnienia | do uzupełnienia |
| 1000 identycznych bajtów | do uzupełnienia | do uzupełnienia | do uzupełnienia | do uzupełnienia |
| Tekst polski / kod źródłowy | do uzupełnienia | do uzupełnienia | do uzupełnienia | do uzupełnienia |
| Losowy plik 1 MB | do uzupełnienia | do uzupełnienia | do uzupełnienia | do uzupełnienia |
| ZIP lub JPG | do uzupełnienia | do uzupełnienia | do uzupełnienia | do uzupełnienia |
| Plik wykonywalny | do uzupełnienia | do uzupełnienia | do uzupełnienia | do uzupełnienia |

### Interpretacja wyników

Interpretację należy przygotować na podstawie uzyskanych pomiarów. Zgodnie
z instrukcją, entropia pliku złożonego z powtórzeń jednego bajtu wynosi zero,
a entropia danych losowych powinna być bliska 8 bitom na symbol. Dla tekstu
naturalnego i kodu źródłowego oczekiwana jest zwykle niższa entropia niż dla
danych losowych lub skompresowanych.

## 9. Wymagania techniczne

- Język C, standard C11.
- Kompilacja z flagami `-std=c11 -Wall -Wextra -O2`.
- Brak ostrzeżeń kompilatora.
- Pliki otwierane w trybie binarnym (`rb` do odczytu, `wb` do zapisu).
- Obsługa błędów wejścia/wyjścia i błędnych argumentów.
- Komunikaty o błędach kierowane na `stderr`.
- Niezerowy kod wyjścia w przypadku błędu.
- Brak wycieków pamięci.

## 10. Struktura projektu

```text
projekt/
├── stderr/
├── tests/
├── bitio.c
├── bitio.h
├── main.c
├── Makefile
├── build.bat
└── README.md
```
