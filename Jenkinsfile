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
}

node ('master') {
  def scmVars = checkout scm
  def x64LinuxReleaseBuildScript = load '.jenkinsci/x64-linux-release-build-steps.groovy'
  def x64LinuxWorker = new Worker(label: 'x86_64', cpusAvailable: 4)

  def x64LinuxReleaseBuildSteps = x64LinuxReleaseBuildScript.doBuild(x64LinuxWorker.label, x64LinuxWorker.cpusAvailable)


  def x64LinuxBuilder = new Builder(buildSteps: x64LinuxReleaseBuildSteps)

  def x64LinuxReleaseBuild = new Build(name: 'x86_64 Linux Release',
                                       type: 'Release',
                                       builder: x64LinuxBuilder,
                                       worker: x64LinuxWorker)

  tasks[x64LinuxReleaseBuild.name] = x64LinuxReleaseBuild.builder.buildSteps

  parallel tasks
}
