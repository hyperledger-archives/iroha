#!/usr/bin/env groovy

def compilerMapping () {
  return ['gcc54': ['cc_compiler':'g++-5', 'cxx_compiler':'gcc-5'],
          'gcc7' : ['cc_compiler':'g++-7', 'cxx_compiler':'gcc-7'],
          'clang6': 'clang']

  }

return this
