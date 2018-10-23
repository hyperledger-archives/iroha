#!/usr/bin/env groovy

def previousCommitOrCurrent() {
  // GIT_PREVIOUS_COMMIT is null on first PR build
  // regardless Jenkins docs saying it equals the current one on first build in branch
  return !env.GIT_PREVIOUS_COMMIT ? env.GIT_COMMIT : env.GIT_PREVIOUS_COMMIT
}

def doDockerCleanup() {

    sh """
      # Check whether the image is the last-standing man
      # i.e., no other tags exist for this image
      docker rmi \$(docker images --no-trunc --format '{{.Repository}}:{{.Tag}}\\t{{.ID}}' | grep \$(docker images --no-trunc --format '{{.ID}}' ${iC.id}) | head -n -1 | cut -f 1) || true
      sleep 5
      docker network rm $IROHA_NETWORK || true
      #remove folder with iroha.deb package and Dockerfiles
      rm -rf /tmp/${env.GIT_COMMIT}-${BUILD_NUMBER}
      rm -rf /tmp/${env.GIT_COMMIT}
    """
}

def selectedBranchesCoverage(branches) {
  return env.GIT_LOCAL_BRANCH in branches
}

def remoteFilesDiffer(f1, f2) {
  sh "curl -sSL -o /tmp/${env.GIT_COMMIT}/f1 --create-dirs ${f1}"
  sh "curl -sSL -o /tmp/${env.GIT_COMMIT}/f2 ${f2}"
  diffExitCode = sh(script: "diff -q /tmp/${env.GIT_COMMIT}/f1 /tmp/${env.GIT_COMMIT}/f2", returnStatus: true)
  sh "rm -f /tmp/${env.GIT_COMMIT}/f1 /tmp/${env.GIT_COMMIT}/f2"
  return  diffExitCode != 0
}

def ccacheSetup(maxSize) {
  sh """
    ccache --version
    ccache --show-stats
    ccache --zero-stats
    ccache --max-size=${maxSize}G
  """
}

return this
