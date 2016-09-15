#ifndef __JAVA_VM_
#define __JAVA_VM_

#include <jni.h>
#include <iostream>
#include <tuple>
#include <memory>

#include <cstdlib>

struct JavaContext{
  JNIEnv* env;
  JavaVM* jvm;
  jclass  jClass;
  jobject jObject;
  std::string name;
  JavaVMInitArgs vmArgs;
  JavaContext(
    JNIEnv* aEnv,
    JavaVM* aJvm,
    JavaVMInitArgs aArgs,
    std::string aName,
    jclass cls,
    jobject obj
  ):
    env(std::move(aEnv)),
    jvm(std::move(aJvm)),
    vmArgs(aArgs),
    name(std::move(aName)),
    jClass(cls),
    jObject(obj)
  {}
};

std::unique_ptr<JavaContext> createVM(std::string contractName);

void execVM(const std::unique_ptr<JavaContext>& context); 

#endif
