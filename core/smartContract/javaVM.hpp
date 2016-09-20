#ifndef __JAVA_VM_
#define __JAVA_VM_

#include <jni.h>
#include <iostream>
#include <tuple>
#include <memory>

#include <unordered_map>

#include <cstdlib>

struct JavaContext {
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
    jClass(std::move(cls)),
    jObject(std::move(obj)),
    name(std::move(aName)),
    vmArgs(std::move(aArgs))
  {}
};

std::unique_ptr<JavaContext> initializeVM(std::string contractName);

void execFunction(const std::unique_ptr<JavaContext>& context,
  std::string functionName,
  std::unordered_map<std::string, std::string> params);


#endif
