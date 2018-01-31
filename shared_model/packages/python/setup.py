import os
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion
import shutil

def dir_up(dir,level):
    if level == 0:
        return dir
    return dir_up(os.path.dirname(os.path.normpath(dir)), level-1)


pwd = os.getcwd()
# IROHA_HOME = "/Users/dumitru/iroha/"
IROHA_HOME = dir_up(pwd,3)

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        for ext in self.extensions:
            self.build_extension(ext)

    def build(self):
        repo_url = "https://github.com/hyperledger/iroha"
        try:
            subprocess.check_call('git clone {} -b develop --depth 1'.format(repo_url).split())
        except:
            pass
    def build_extension(self, ext):
        print(ext.sourcedir)
        self.build()
        #sys.exit()

        # cmd = "cmake -H"+IROHA_HOME+" -Bbuild -DSWIG_PYTHON=ON"
        # subprocess.check_call(cmd.split())
        # subprocess.check_call("cmake --build build/ --target irohapy -- -j4".split())
        # git.Git('.').clone("git://gitorious.org/git-python/mainline.git")

        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]


        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg, '-DSWIG_PYTHON=ON']
        build_args += '--target irohapy -- -j4'.split(' ')

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)
        shutil.copy(self.build_temp+"/shared_model/bindings/iroha.py", extdir+"/")

        pwd = os.getcwd()
        print(subprocess.check_call(["pwd"]))
        print(subprocess.check_call(["ls"]))
        os.chdir(extdir)
        gen_proto = "protoc --proto_path={}/iroha/schema --python_out=. block.proto primitive.proto commands.proto queries.proto responses.proto endpoint.proto".format(pwd)
        subprocess.check_call(gen_proto.split())
        gen_py = "python -m grpc_tools.protoc --proto_path={}/iroha/schema --python_out=. --grpc_python_out=. endpoint.proto yac.proto ordering.proto loader.proto".format(pwd)
        subprocess.check_call(gen_py.split())
        os.chdir(pwd)


# subprocess.check_call()

if __name__ == "__main__":
    setup(
        name='iroha',
        version='0.0.4',
        author='Soramitsu',
        author_email='soramitsu@gmail.com',
        description='Python library for iroha',
        long_description='',
        ext_modules=[CMakeExtension('iroha', 'iroha')],
        cmdclass=dict(build_ext=CMakeBuild),
        install_requires=[
            "grpcio-tools",
        ],
        zip_safe=False,
    )