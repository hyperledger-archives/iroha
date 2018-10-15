// @Library('ci-lib')
// import jp.co.soramitsu.TestClass.*

class Worker {
  String label
  int cpusAvailable
}

def t = new Worker(label: 'LABEL', cpusAvailable: 8)

node('master') {
  def scmVars = checkout scm
  def a = t.label
  sh "echo ${a}"
}
