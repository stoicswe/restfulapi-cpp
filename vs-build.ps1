Write-Output("Starting setup...")

$ROOT = $PWD
$Build = $PWD.Path + "\build"
$Install = $PWD.Path + "\install"

#New-Item -ItemType Directory -Force -Path $Folder
Write-Output("Starting conan install")

conan install . `
    -if $Build `
    -s build_type=Release `
    -s compiler="Visual Studio" `
    -s compiler.runtime=MDd `
    -s compiler.cppstd=20 `
    -s arch=x86_64 `
    --update `
    --build=missing

conan install . `
    -if $Build `
    -s build_type=Debug `
    -s compiler="Visual Studio" `
    -s compiler.runtime=MDd `
    -s compiler.cppstd=20 `
    -s arch=x86_64 `
    --update `
    --build=missing

Write-Output("Cmake build")
#cd $Folder
#cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
#cmake --build . --config Release
conan build -if $Build -bf $Build -pf $Install -c -b -t "$PWD/conanfile.py"
Set-Location $ROOT
