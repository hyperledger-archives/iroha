def tasks = [:]

class Worker {
  String label
  int cpusAvailable
}

class Builder {
  class PostSteps {
    List success
    List failure
    List unstable
    List always
    List aborted
  }
  List buildSteps
  PostSteps postSteps
}

class Build {
  String name
  String type
  Builder builder
  Worker worker

  def build() {
    try {
      this.builder.buildSteps
      if (currentBuild.currentResult == 'SUCCESS') {
        this.builder.postSteps.success
      }
      else if(currentBuild.currentResult == 'UNSTABLE') {
        this.builder.postSteps.unstable
      }
      else if(currentBuild.currentResult == 'FAILURE') {
        this.builder.postSteps.failure
      }
      else if(currentBuild.currentResult == 'ABORTED') {
        this.builder.postSteps.aborted
      }
    }
    finally {
      this.builder.postSteps.always
    }
  }
}

node ('master') {
  scmVars = checkout scm
  environmentList = []
  environment = [:]
  environment = [
    "CCACHE_DEBUG_DIR": "/opt/.ccache",
    "CCACHE_RELEASE_DIR": "/opt/.ccache",
    "DOCKER_REGISTRY_BASENAME": "hyperledger/iroha",
    "IROHA_NETWORK": "iroha-${scmVars.CHANGE_ID}-${scmVars.GIT_COMMIT}-${env.BUILD_NUMBER}",
    "IROHA_POSTGRES_HOST": "pg-${scmVars.CHANGE_ID}-${scmVars.GIT_COMMIT}-${env.BUILD_NUMBER}",
    "IROHA_POSTGRES_USER": "pguser${scmVars.GIT_COMMIT}",
    "IROHA_POSTGRES_PASSWORD": "${scmVars.GIT_COMMIT}",
    "GIT_RAW_BASE_URL": "https://raw.githubusercontent.com/hyperledger/iroha"
  ]
  environment.each { e ->
    environmentList.add("${e.key}=${e.value}")
  }

  x64LinuxReleaseBuildScript = load '.jenkinsci/builders/x64-linux-release-build-steps.groovy'
  x64LinuxDebugBuildScript = load '.jenkinsci/builders/x64-linux-debug-build-steps.groovy'
  x64LinuxWorker = new Worker(label: 'x86_64', cpusAvailable: 4)
  // def x64MacWorker = new Worker(label: 'mac', cpusAvailable: 4)

  x64LinuxReleaseBuildSteps = x64LinuxReleaseBuildScript.buildSteps(
    x64LinuxWorker.label, x64LinuxWorker.cpusAvailable, 'gcc54', 'develop', false, environmentList)
  x64LinuxReleasePostSteps = new Builder.PostSteps(
    always: [x64LinuxReleaseBuildScript.alwaysPostSteps(environmentList)],
    success: [x64LinuxReleaseBuildScript.successPostSteps(scmVars, environmentList)])

  // x64LinuxDebugBuildSteps = x64LinuxDebugBuildScript.buildSteps(nodeLabel=x64LinuxWorker.label,
  //   parallelism=x64LinuxWorker.cpusAvailable, compilerVersion='gcc54', pushDockerTag=true, coverage=false,
  //   testing=false, cppcheck=true, sonar=false, environment=environmentList)
  // x64LinuxDebugPostSteps = new x64LinuxDebugBuildSteps.PostSteps(
  //   always: [x64LinuxDebugBuildScript.alwaysPostSteps(environmentList)])
  //def x64MacReleaseBuildSteps = x64LinuxReleaseBuildScript.buildSteps(x64MacWorker.label, x64MacWorker.cpusAvailable)

  x64LinuxReleaseBuilder = new Builder(buildSteps: x64LinuxReleaseBuildSteps, postSteps: x64LinuxReleasePostSteps)
  //x64LinuxDebugBuilder = new Builder(buildSteps: x64LinuxDebugBuildSteps, postSteps: x64LinuxDebugPostSteps)
  //def x64MacBuilder = new Builder(buildSteps: x64MacReleaseBuildSteps)

  x64LinuxReleaseBuild = new Build(name: 'x86_64 Linux Release',
                                       type: 'Release',
                                       builder: x64LinuxReleaseBuilder,
                                       worker: x64LinuxWorker)

  // x64LinuxDebugBuild = new Build(name: 'x86_64 Linux Debug',
  //                                      type: 'Debug',
  //                                      builder: x64LinuxDebugBuilder,
  //                                      worker: x64LinuxWorker)
  // def x64MacReleaseBuild = new Build(name: 'Mac Linux Release',
  //                                    type: 'Release',
  //                                    builder: x64MacBuilder,
  //                                    worker: x64MacWorker)

  tasks[x64LinuxReleaseBuild.name] = { x64LinuxReleaseBuild.build() }
  //tasks[x64LinuxDebugBuild.name] = { x64LinuxDebugBuild.build() }
  //tasks[x64MacReleaseBuild.name] = { x64MacReleaseBuild.build() }

  parallel tasks

  // if (currentBuild.currentResult == 'SUCCESS') {
  //   if (scmVars.CHANGE_ID) {
  //     if(scmVars.CHANGE_BRANCH == 'feature/ready-dev-experimental') {
  //       sh 'echo PUSH!'
  //     }
  //   }
  // }
}
