# -*- coding: utf-8 -*-
from config.fabric import myhosts, port, user, github, repo_name
from config.slack  import notify_slack
 
from fabric.api import *
from fabric.colors import *

import slackweb
import random

env.hosts = myhosts
env.port =  port
env.user =  user

@task
def status():
  print(magenta("+++++++++++++++++++"))
  print(magenta("+ *    *       *  +"))
  print(magenta("+  server status! +"))
  print(magenta("+  *            * +"))
  print(magenta("+++++++++++++++++++"))
  run("cmake --version")
  run("gcc --version")
  run("make --version")

  with shell_env(JAVA_HOME='/usr/lib/jvm/java-8-openjdk-amd64'):
    run("java -version")
    run("javac -version")
    run("gradle -v")


@task
def initalize_server():
  print(magenta("##########################"))
  print(magenta("# Setup server!! \(^o^)/ #"))
  print(magenta("##########################"))
  print(cyan("#  install usual"))
  sudo("apt -y install gcc g++ make git")
  print(cyan("#  use for crow"))
  sudo("apt -y install build-essential libtcmalloc-minimal4 && sudo ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so", warn_only=True)
  print(cyan("#  booost"))
  sudo("apt -y install libboost-all-dev")
  print(cyan("#  cmake"))
  sudo("apt -y install cmake")
  print(cyan("#  Java"))
  sudo("apt -y install default-jdk")
  sudo("apt -y install default-jre") 
  print(cyan("# other libs"))
  sudo("apt -y install libssl-dev")
  sudo("apt -y install unzip")
  run("curl -s https://get.sdkman.io | bash")
  run('source "/home/'+env.user+'/.sdkman/bin/sdkman-init.sh" && sdk install gradle 3.0')
  status()
  run('git config --global user.email ' + github.email)
  run('git config --global user.name  ' + github.name)
  sudo("mkdir -p /var/www")


def git_current_branch():
  res = local('git branch --contains', capture=True)
  return res.split(' ')[1]

@task
def check_key_github():
  with quiet():
    res = run("ssh -T git@github.com", warn_only=True)
    if not "successfully authenticated" in res:
      print(red("Githubにサーバーの公開鍵が登録されていないよ!!"))
      print(red("gitHubでssh接続する手順~公開鍵・秘密鍵の生成から~"))
      print("Ref:http://qiita.com/shizuma/items/2b2f873a0034839e47ce")
      quit()

@task
def initialize_repository():
  check_key_github()
  with cd("/var/www"):
    sudo("rm -rf *")
    sudo("git clone --recursive "+repo_name)
  with cd("/var/www/iroha"):
    with shell_env(JAVA_HOME='/usr/lib/jvm/java-8-openjdk-amd64'):
      with cd("core/vendor/Aeron"):
        sudo("./gradlew")
        sudo("mkdir -p cppbuild/Debug")
        with cd("cppbuild/Debug"):
          sudo("cmake ../..")
          sudo("cmake --build . --clean-first")
          sudo("ctest")

      with cd("core/vendor/leveldb"):
        sudo("make")
      
      with cd("core/vendor/ed25519"):
        sudo("make")

      with cd("core/vendor/msgpack-c"):
        sudo("cmake -DMSGPACK_CXX11=ON .")
        sudo("cmake -DMSGPACK_CXX11=ON .")
        sudo("make install")
        
      with cd("core/vendor/yaml-cpp"):
        sudo("mkdir -p build")
        with cd("build"):
          sudo("cmake ..")
          sudo("make")
        
      with cd("core/vendor/crow"):
        sudo("mkdir -p build")
        with cd("build"):
          sudo("cmake ..")
          sudo("make")
           

@task
def connection_test_dev():
  run("cat welcome")
  print(git_current_branch())

@task
def connection_test_stage():
  run("cat welcome")

@task
def connection_test_production():
  run("cat welcome")


def remake():
  with cd("/var/www/iroha"):
    sudo("mkdir -p build")
    with cd("build"):
      sudo("cmake ..")
      sudo("make")

def restart():
  with cd("/var/www/iroha"):
    sudo('pkill -f "iroha-main"')
    sudo('./build/bin/iroha-main &')
    

@task
def test(branch = None):
  print(blue("#################"))
  print(blue("# test    (^o^) #"))
  print(blue("#################"))
  connection_test_dev()
  if not branch:
    branch = git_current_branch()
    with cd("/var/www/iroha"):
      res = sudo("git reset --hard")
      res = sudo("git checkout -b "+branch+" origin/"+branch, warn_only=True)
      if res.failed:
        sudo("git checkout "+branch) 
      sudo("git pull origin "+branch+" --no-ff")
       
      remake()
      restart()















#
# 

@task
def deploy_stage_with_circle_ci():
  print(yellow("#################"))
  print(yellow("# Staging !!!!  #"))
  print(yellow("#################"))
  env.hosts = ['45.32.49.154']
  env.port = '1225'
  env.user = 'deploy'
  connection_test_stage()

@task
def deploy_with_circle_ci():
  print(red("#################"))
  print(red("# Production !! #"))
  print(red("#################"))
  env.hosts = ['45.32.49.154']
  env.port = '1225'
  env.user = 'deploy'
  connection_test_production()
