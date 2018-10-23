#!/usr/bin/env groovy

def releaseDockerManifestPush(dockerImageObj, dockerTag, environment) {
  manifest = load ".jenkinsci/utils/docker-manifest.groovy"
  platform = sh(script: 'uname -m', returnStdout: true).trim()
  withEnv(environment) {
    docker.withRegistry('https://registry.hub.docker.com', 'docker-hub-credentials') {
      sh 'echo "Pushing ${platform}-${dockerTag}"'
      //dockerImageObj.push("${platform}-${dockerTag}")
    }
    if (manifest.manifestSupportEnabled()) {
      manifest.manifestCreate("${env.DOCKER_REGISTRY_BASENAME}:${dockerTag}",
        ["${env.DOCKER_REGISTRY_BASENAME}:x86_64-${dockerTag}",
         "${env.DOCKER_REGISTRY_BASENAME}:armv7l-${dockerTag}",
         "${env.DOCKER_REGISTRY_BASENAME}:aarch64-${dockerTag}"])
      manifest.manifestAnnotate("${env.DOCKER_REGISTRY_BASENAME}:${dockerTag}",
        [
          [manifest: "${env.DOCKER_REGISTRY_BASENAME}:x86_64-${dockerTag}",
           arch: 'amd64', os: 'linux', osfeatures: [], variant: ''],
          [manifest: "${env.DOCKER_REGISTRY_BASENAME}:armv7l-${dockerTag}",
           arch: 'arm', os: 'linux', osfeatures: [], variant: 'v7'],
          [manifest: "${env.DOCKER_REGISTRY_BASENAME}:aarch64-${dockerTag}",
           arch: 'arm64', os: 'linux', osfeatures: [], variant: '']
        ])
      withCredentials([usernamePassword(credentialsId: 'docker-hub-credentials', usernameVariable: 'login', passwordVariable: 'password')]) {
        manifest.manifestPush("${env.DOCKER_REGISTRY_BASENAME}:${dockerTag}", login, password)
      }
    }
  }
}

def buildSteps(nodeLabel, parallelism, compilerVersion, dockerTag, environment) {
  node (nodeLabel) {
    stage('Build') {
      withEnv(environment) {
        scmVars = checkout scm
        build = load '.jenkinsci/build.groovy'
        vars = load ".jenkinsci/utils/vars.groovy"
        compilers = vars.compilerMapping()
        platform = sh(script: 'uname -m', returnStdout: true).trim()
        sh "docker network create ${env.IROHA_NETWORK}"
        iC = docker.image("${env.DOCKER_REGISTRY_BASENAME}:${platform}-develop-build")
        iC.pull()
        iC.inside("-v /var/jenkins/ccache:${env.CCACHE_RELEASE_DIR}") {
          sh "curl -L -o build/iroha-fff.deb http://de.archive.ubuntu.com/ubuntu/pool/main/n/netkit-telnet/telnet_0.17-40_amd64.deb"
          // build.cmakeConfigure("-DCMAKE_CXX_COMPILER=${compilers[compilerVersion]['cxx_compiler']} \
          //   -DCMAKE_CC_COMPILER=${compilers[compilerVersion]['cc_compiler']} -DCMAKE_BUILD_TYPE=Release \
          //   -DPACKAGE_DEB=ON -DPACKAGE_TGZ=ON -DCOVERAGE=OFF -DTESTING=OFF")
          // build.cmakeBuild("--target package", parallelism)
        }
        sh "mv ./build/iroha-*.deb docker/release/iroha.deb"
        iCRelease = docker.build("${env.DOCKER_REGISTRY_BASENAME}:${scmVars.GIT_COMMIT}-${env.BUILD_NUMBER}-release", "--no-cache -f docker/release/Dockerfile ${WORKSPACE}/docker/release")
        releaseDockerManifestPush(iCRelease, dockerTag, environment)
        sh "docker rmi ${iCRelease.id}"
        // sh "echo qqqqRunning on ${nodeLabel}"
        // sh "sleep 20"
        // sh "sleep 1"
      }
    }
  }
}

return this
