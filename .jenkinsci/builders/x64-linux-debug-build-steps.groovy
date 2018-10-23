#!/usr/bin/env groovy

def debugBuildDockerManifestPush() {
  manifest.manifestCreate("${DOCKER_REGISTRY_BASENAME}:develop-build",
    ["${DOCKER_REGISTRY_BASENAME}:x86_64-develop-build",
     "${DOCKER_REGISTRY_BASENAME}:armv7l-develop-build",
     "${DOCKER_REGISTRY_BASENAME}:aarch64-develop-build"])
  manifest.manifestAnnotate("${DOCKER_REGISTRY_BASENAME}:develop-build",
    [
      [manifest: "${DOCKER_REGISTRY_BASENAME}:x86_64-develop-build",
       arch: 'amd64', os: 'linux', osfeatures: [], variant: ''],
      [manifest: "${DOCKER_REGISTRY_BASENAME}:armv7l-develop-build",
       arch: 'arm', os: 'linux', osfeatures: [], variant: 'v7'],
      [manifest: "${DOCKER_REGISTRY_BASENAME}:aarch64-develop-build",
       arch: 'arm64', os: 'linux', osfeatures: [], variant: '']
    ])
  withCredentials([usernamePassword(credentialsId: 'docker-hub-credentials', usernameVariable: 'login', passwordVariable: 'password')]) {
    manifest.manifestPush("${DOCKER_REGISTRY_BASENAME}:develop-build", login, password)
  }
}

def buildSteps(String nodeLabel, int parallelism, String compilerVersion) {
  node (nodeLabel) {
    stage('Build') {
      def scmVars = checkout scm
      def build = load '.jenkinsci/build.groovy'
      sh "docker network create ${env.IROHA_NETWORK}"
      iC = docker.image("hyperledger/iroha:x86_64-develop-build")
      iC.pull()
      iC.inside {
        build.cmakeConfigure("-DCMAKE_BUILD_TYPE=Release")
        build.cmakeBuild("", 8)

      }
      // sh "echo qqqqRunning on ${nodeLabel}"
      // sh "sleep 20"
      // sh "sleep 1"
    }
  }
}

return this
