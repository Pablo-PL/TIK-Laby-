#!/bin/bash
set -u

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR" || exit 1

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

echo "========================================"
echo "                 TESTY"
echo "========================================"

# Test 1: pusty plik
: > "$TMP_DIR/pusty.bin"
./program "$TMP_DIR/pusty.bin" > "$TMP_DIR/pusty.out" 2> "$TMP_DIR/pusty.err"
if grep -q "Rozmiar pliku: 0 bajtow" "$TMP_DIR/pusty.out"; then
    echo "[OK] Pusty plik"
else
    echo "[BLAD] Pusty plik"
    exit 1
fi

# Test 2: tekst o znanym rozkladzie bajtow
printf "AAAAABBBCC" > "$TMP_DIR/prosty.txt"
./program "$TMP_DIR/prosty.txt" > "$TMP_DIR/prosty.out" 2> "$TMP_DIR/prosty.err"
if grep -q "Rozmiar pliku: 10 bajtow" "$TMP_DIR/prosty.out" &&
   grep -q "0x41 : 5" "$TMP_DIR/prosty.out" &&
   grep -q "0x42 : 3" "$TMP_DIR/prosty.out" &&
   grep -q "0x43 : 2" "$TMP_DIR/prosty.out"; then
    echo "[OK] Zliczanie bajtow i rozmiar"
else
    echo "[BLAD] Zliczanie bajtow i rozmiar"
    exit 1
fi

# Test 3: dane binarne, w tym bajt 0x00 i 0xFF
printf '\x00\x01\x02\xFF\x00' > "$TMP_DIR/binarny.bin"
./program "$TMP_DIR/binarny.bin" > "$TMP_DIR/binarny.out" 2> "$TMP_DIR/binarny.err"
if grep -q "Rozmiar pliku: 5 bajtow" "$TMP_DIR/binarny.out" &&
   grep -q "0xFF : 1" "$TMP_DIR/binarny.out"; then
    echo "[OK] Odczyt danych binarnych"
else
    echo "[BLAD] Odczyt danych binarnych"
    exit 1
fi

# Test 4: nieistniejacy plik
if ./program "$TMP_DIR/nieistnieje.bin" > "$TMP_DIR/brak.out" 2> "$TMP_DIR/brak.err"; then
    echo "[BLAD] Brak pliku powinien zwrocic kod niezerowy"
    exit 1
else
    echo "[OK] Obsluga braku pliku"
fi

# Test 5: niepoprawna liczba argumentow
if ./program jeden dwa > "$TMP_DIR/args.out" 2> "$TMP_DIR/args.err"; then
    echo "[BLAD] Bledne argumenty powinny zwrocic kod niezerowy"
    exit 1
else
    echo "[OK] Obsluga blednych argumentow"
fi

echo "========================================"
echo "Wszystkie testy zakonczone powodzeniem."
echo "========================================"
