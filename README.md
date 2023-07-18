# restfulapi-cpp
This is an example project for setting up conan/cmake for cpp development, using libcurl for HTTP calls and PLOG for logging.

If working with CLion, use the following command prior to running built-in cmake build tool:
```powershell
conan install . -if ./build --update --build=missing
```