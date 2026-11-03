$ErrorActionPreference = "Stop"

# Uruchamiaj skrypt z glownego katalogu projektu:
# powershell -ExecutionPolicy Bypass -File tests\run.ps1

$Root = (Get-Location).Path
$Tests = Join-Path $Root "tests"
$Entropy = Join-Path $Root "entropy.exe"
$Results = Join-Path $Tests "wyniki_pomiarow.txt"

if (-not (Test-Path $Entropy)) {
    Write-Error "Nie znaleziono entropy.exe w glownym katalogu. Najpierw skompiluj projekt."
    exit 1
}

# 1. Pusty plik
[System.IO.File]::WriteAllBytes((Join-Path $Tests "pusty.bin"), [byte[]]@())

# 2. 1000 powtorzen bajtu 0x41 ('A')
$repeated = New-Object byte[] 1000
for ($i = 0; $i -lt $repeated.Length; $i++) {
    $repeated[$i] = 0x41
}
[System.IO.File]::WriteAllBytes((Join-Path $Tests "powtorzenia.bin"), $repeated)

# 3. Tekst testowy (UTF-8 bez BOM)
$text = @'
Teoria informacji i kodowania.
Entropia opisuje srednia ilosc informacji przypadajaca na symbol.
Ten plik sluzy do sprawdzenia programu analizujacego bajty.
'@
[System.IO.File]::WriteAllText((Join-Path $Tests "tekst.txt"), $text, [System.Text.UTF8Encoding]::new($false))

# 4. Losowy plik 1 MiB – zgodnie z instrukcja laboratorium
$randomBytes = New-Object byte[] (1MB)
$rng = [System.Random]::new()
$rng.NextBytes($randomBytes)
[System.IO.File]::WriteAllBytes((Join-Path $Tests "losowy.bin"), $randomBytes)

# 5. Archiwum ZIP utworzone z tekstu testowego
$zipPath = Join-Path $Tests "archiwum.zip"
if (Test-Path $zipPath) {
    Remove-Item $zipPath -Force
}
Compress-Archive -Path (Join-Path $Tests "tekst.txt") -DestinationPath $zipPath -CompressionLevel Optimal

# 6. Program wykonywalny – kopia entropy.exe
Copy-Item $Entropy (Join-Path $Tests "entropy_test.exe") -Force

$cases = @(
    @{ Name = "Pusty plik"; Path = (Join-Path $Tests "pusty.bin") },
    @{ Name = "1000 identycznych bajtow"; Path = (Join-Path $Tests "powtorzenia.bin") },
    @{ Name = "Tekst"; Path = (Join-Path $Tests "tekst.txt") },
    @{ Name = "Losowy plik 1 MiB"; Path = (Join-Path $Tests "losowy.bin") },
    @{ Name = "Archiwum ZIP"; Path = $zipPath },
    @{ Name = "Plik wykonywalny"; Path = (Join-Path $Tests "entropy_test.exe") }
)

$allOutput = @()
$allOutput += "Wyniki testow programu entropy"
$allOutput += "Data: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
$allOutput += ""

foreach ($case in $cases) {
    Write-Host ""
    Write-Host "========== $($case.Name) ==========" -ForegroundColor Cyan

    $output = & $Entropy $case.Path 2>&1
    $exitCode = $LASTEXITCODE

    $output | ForEach-Object { Write-Host $_ }

    if ($exitCode -ne 0) {
        Write-Error "Test nie powiodl sie: $($case.Name), kod wyjscia: $exitCode"
        exit 1
    }

    $allOutput += "========== $($case.Name) =========="
    $allOutput += $output | ForEach-Object { "$_" }
    $allOutput += ""
}

$allOutput | Set-Content -Path $Results -Encoding UTF8

Write-Host ""
Write-Host "Testy zakonczone. Wyniki zapisano w: $Results" -ForegroundColor Green
