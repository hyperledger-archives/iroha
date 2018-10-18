def tasks = [:]

class Worker {
  String label
  int cpusAvailable
}

class Builder {
  List buildSteps
}

class Build {
  String name
  String type
  Builder builder
  Worker worker

  def build() {
    this.builder.buildSteps
  }
}

node ('master') {
  def scmVars = checkout scm
  def x64LinuxReleaseBuildScript = load '.jenkinsci/builders/x64-linux-release-build-steps.groovy'
  def x64LinuxWorker = new Worker(label: 'x86_64', cpusAvailable: 4)
  def x64MacWorker = new Worker(label: 'mac', cpusAvailable: 4)

  def x64LinuxReleaseBuildSteps = x64LinuxReleaseBuildScript.buildSteps(x64LinuxWorker.label, x64LinuxWorker.cpusAvailable)
  def x64MacReleaseBuildSteps = x64LinuxReleaseBuildScript.buildSteps(x64MacWorker.label, x64MacWorker.cpusAvailable)

  def x64LinuxBuilder = new Builder(buildSteps: x64LinuxReleaseBuildSteps)
  def x64MacBuilder = new Builder(buildSteps: x64MacReleaseBuildSteps)

  def x64LinuxReleaseBuild = new Build(name: 'x86_64 Linux Release',
                                       type: 'Release',
                                       builder: x64LinuxBuilder,
                                       worker: x64LinuxWorker)
  def x64MacReleaseBuild = new Build(name: 'Mac Linux Release',
                                     type: 'Release',
                                     builder: x64MacBuilder,
                                     worker: x64MacWorker)

  tasks[x64LinuxReleaseBuild.name] = { x64LinuxReleaseBuild.build() }
  //tasks[x64MacReleaseBuild.name] = { x64MacReleaseBuild.build() }

  parallel tasks

  if (currentBuild.currentResult == 'SUCCESS') {
    if (scmVars.CHANGE_ID) {
      if(scmVars.CHANGE_BRANCH == 'feature/ready-dev-experimental') {
        sh 'echo PUSH!'
      }
    }
  }
}
