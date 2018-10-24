#!/usr/bin/env groovy

def previousCommitOrCurrent(scmVars) {
  // GIT_PREVIOUS_COMMIT is null on first PR build
  // regardless Jenkins docs saying it equals the current one on first build in branch
  return !scmVars.GIT_PREVIOUS_COMMIT ? scmVars.GIT_COMMIT : scmVars.GIT_PREVIOUS_COMMIT
}

def doDockerCleanup() {
  // Check whether the image is the last-standing man
  // i.e., no other tags exist for this image
  sh """
    docker rmi \$(docker images --no-trunc --format '{{.Repository}}:{{.Tag}}\\t{{.ID}}' | grep \$(docker images --no-trunc --format '{{.ID}}' ${iC.id}) | head -n -1 | cut -f 1) || true
    docker network rm $IROHA_NETWORK || true
  """
}

def selectedBranchesCoverage(branches) {
  return env.GIT_LOCAL_BRANCH in branches
}

def ccacheSetup(maxSize) {
  sh """
    ccache --version
    ccache --show-stats
    ccache --zero-stats
    ccache --max-size=${maxSize}G
  """
}

def dockerPush(dockerImageObj, imageName) {
  docker.withRegistry('https://registry.hub.docker.com', 'docker-hub-credentials') {
    dockerImageObj.push(imageName)
  }
}

def getUrl(String url, String savePath, boolean createDstDir=false) {
  if (createDstDir) {
    sh "curl -L -o ${savePath} --create-dirs ${url}"
  }
  else {
    sh "curl -L -o ${savePath} ${url}"
  }
  return savePath
}

def filesDiffer(f1, f2) {
  diffExitCode = sh(script: "diff -q ${f1} ${f2}", returnStatus: true)
  return diffExitCode != 0
}

return this
