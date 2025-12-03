# Script to copy required DLLs to bin directory
# Run this after building the project

$binDir = "bin"
$msys2Bin = "C:\msys64\mingw64\bin"

# Create bin directory if it doesn't exist
if (-not (Test-Path $binDir)) {
    New-Item -ItemType Directory -Path $binDir | Out-Null
}

# List of required DLLs
$requiredDlls = @(
    "libassimp-6.dll",
    "libcurl-4.dll",
    "libfreetype-6.dll",
    "libfreeimage-3.dll",
    "libopenal-1.dll",
    "librtaudio-7.dll",
    "libsndfile-1.dll",
    "libmpg123-0.dll",
    "libssl-3-x64.dll",
    "libcrypto-3-x64.dll",
    "zlib1.dll",
    "libpng16-16.dll"
)

Write-Host "Copying DLLs from $msys2Bin to $binDir..."

$copied = 0
$missing = 0

foreach ($dll in $requiredDlls) {
    $src = Join-Path $msys2Bin $dll
    $dst = Join-Path $binDir $dll
    
    if (Test-Path $src) {
        Copy-Item -Path $src -Destination $dst -Force -ErrorAction SilentlyContinue
        Write-Host "  [OK] $dll"
        $copied++
    } else {
        Write-Host "  [MISSING] $dll"
        $missing++
    }
}

Write-Host "`nDone! Copied $copied DLLs, $missing missing."


