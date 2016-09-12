#include <jni.h>
#include <iostream>
#include <tuple>
#include <memory>


struct JavaContext{
  JNIEnv* env;
  JavaVM* jvm;
  std::string name;
  JavaVMInitArgs vmArgs;
  JavaContext(
    JNIEnv* aEnv,
    JavaVM* aJvm,
    JavaVMInitArgs aArgs,
    std::string aName
  ):
    env(std::move(aEnv)),
    jvm(std::move(aJvm)),
    vmArgs(aArgs),
    name(std::move(aName))
  {}
};

void void Java_SmartContract_save(JNIEnv *env,jobject thiz,jstring key,jstring value){
    const char *keyChar   = env->GetStringUTFChars(   key,0);
    const char *valueChar = env->GetStringUTFChars( value,0);

    std::cout << "[" << keyChar <<", "<< valueChar << std::endl;

    env->ReleaseStringUTFChars(  key,   keyChar);
    env->ReleaseStringUTFChars(value, valueChar);
}

std::unique_ptr<JavaContext> createVM(std::string contractName){
  JNIEnv* env;
  JavaVM* jvm;

  JavaVMOption options[1];
  options[0].optionString = (char*)"-Djava.security.manager -Djava.security.policy=policy.txt -Djava.class.path=.";

  JavaVMInitArgs vm_args;
  vm_args.version = JNI_VERSION_1_6;
  vm_args.options = options;
  vm_args.nOptions = 1;
  //vm_args.ignoreUnrecognized = true;

  int res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
  if(res) {
    std::cout << "cannot run JavaVM : "<< res << std::endl;
    return nullptr;
  }
  return std::make_unique<JavaContext>(
    env,
    jvm,
    vm_args,
    std::move(contractName)
  );
}

void execVM(const std::unique_ptr<JavaContext> context){
  int res = JNI_CreateJavaVM(&context->jvm, (void**)&context->env, &context->vmArgs);
  if(res){
    std::cout << "cannot run JavaVM : " << res << std::endl;
    return;
  }

  jclass cls = context->env->FindClass("SmartContract");
  if(cls){
    std::cout << "could not found class : SmartContract" << std::endl;
    return;
  }

  jmethodID init = context->env->GetMethodID(cls, "<init>", "()V");
  if(init){
    return;
  }
  jobject obj = context->env->NewObject(cls, init);

  jmethodID mid = context->env->GetStaticMethodID(cls, "remit", "(Ljava/util/Map;)V");
  if(mid == NULL){
    std::cout << "could not get method : " << "remit" << std::endl;
    return;
  }

  context->env->CallVoidMethod(obj, mid);

  if(context->jvm->DestroyJavaVM()){
    std::cout << "could not destroy JavaVM : " << res << std::endl;
  }
}
