#!/usr/bin/env groovy

def cmakeConfigure(String cmakeOptions) {
  sh "cmake -H. -Bbuild ${cmakeOptions}"
}

def cmakeBuild(String cmakeOptions, int parallelism) {
  sh "cmake --build build ${cmakeOptions} -- -j${parallelism}"
}

return this
