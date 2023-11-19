buildType=${1:-"debug"}
buildMake=${2:-false}
additionalArgs=${3:-""}
if [ "${buildType}" == "debug" ]
then
  if [ -d "./build" ]
  then
    rm -rf ./build
    mkdir ./build
    conan install . -s build_type=Debug --output-folder=build $additionalArgs
  else
    echo "Debug build clean."
    conan install . -s build_type=Debug --output-folder=build $additionalArgs
  fi
  else
  if [ -d "./build" ]
  then
    rm -rf ./build
    mkdir ./build
    conan install . -s build_type=Release --output-folder=build $additionalArgs
  else
    echo "Release build clean."
    conan install . -s build_type=Release --output-folder=build $additionalArgs
  fi
fi
#conan profile new default --detect
#conan profile update settings.compiler.libcxx=libstdc++11 default
#conan install .. --build=missing
if [ $buildMake == true ]
then
  cd ./build
  cmake .. -G "Xcode" -DCMAKE_BUILD_TYPE=Release
  cmake --build . --config "${buildType}"
  echo "Done."
else
  echo "Done."
fi
cd ..