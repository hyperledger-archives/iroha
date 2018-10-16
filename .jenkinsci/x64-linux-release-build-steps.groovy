#!/usr/bin/env groovy

def doBuild(String nodeLabel, int parallelism) {
  node (nodeLabel) {
    def scmVars = checkout scm
    def build = load '.jenkinsci/build.groovy'
    iC = docker.image("hyperledger/iroha:x86_64-develop-build")
    iC.pull()
    iC.inside {
      build.cmakeConfigure("-DCMAKE_RELEASE_TYPE=Release")
      build.cmakeBuild("", parallelism)
    }
  }
}

return this
