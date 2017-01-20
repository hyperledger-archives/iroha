/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "java_virtual_machine.hpp"
#include <algorithm>
#include <array>

namespace smart_contract {

    void Java_SmartContract_save(JNIEnv *env, jobject thiz, jstring key, jstring value) {
        const char *keyChar = env->GetStringUTFChars(key, 0);
        const char *valueChar = env->GetStringUTFChars(value, 0);

        std::cout << "[" << keyChar << ", " << valueChar << std::endl;

        env->ReleaseStringUTFChars(key, keyChar);
        env->ReleaseStringUTFChars(value, valueChar);
    }

    std::unique_ptr<JavaContext> initializeVM(std::string contractName) {

        if (getenv("IROHA_HOME") == nullptr) {
            std::cout << "You must set IROHA_HOME!" << std::endl;
            return nullptr;
        }

        std::string java_command = "-Djava.class.path=" + std::string(getenv("IROHA_HOME")) + "/smart_contract/";
        std::string java_lib_path = "-Djava.library.path=" + std::string(getenv("IROHA_HOME")) + "/smart_contract/";

    		JavaVMOption options[4];
    		options[0].optionString = const_cast<char*>( java_command.c_str() );
    		options[1].optionString = const_cast<char*>("-Djava.security.manager");
    		options[2].optionString = const_cast<char*>("-Djava.security.policy=policy.txt");
        options[3].optionString = const_cast<char*>( java_lib_path.c_str() );

        JavaVMInitArgs vm_args;
        vm_args.version  = JNI_VERSION_1_6;
        vm_args.options  = options;
        vm_args.nOptions = 4;
        //vm_args.ignoreUnrecognized = true;

        JNIEnv *env;
        JavaVM *jvm;

        int res = JNI_CreateJavaVM(&jvm, (void **) &env, &vm_args);
        if (res) {
            std::cout << "cannot run JavaVM : " << res << std::endl;
            return nullptr;
        }
        std::string package_name = contractName;
        std::transform(package_name.begin(), package_name.end(), package_name.begin(), ::tolower);
        jclass cls = env->FindClass( (package_name + "/" +contractName).c_str() );// (contractName+"/"+contractName).c_str());
        if (cls == nullptr) {
            std::cout << "could not found class : " << contractName << std::endl;
            return nullptr;
        }

        jmethodID cns = env->GetMethodID(cls, "<init>", "()V");
        if (cns == nullptr) {
            std::cout << "could not get <init> method." << std::endl;
            return nullptr;
        }

        jobject obj = env->NewObject(cls, cns);

        return std::make_unique<JavaContext>(
            env,
            jvm,
            vm_args,
            std::move(contractName),
            cls,
            obj
        );
    }



    void execFunction(
        const std::unique_ptr<JavaContext> &context,
        std::string functionName,
        std::unordered_map<std::string, std::string> params
    ) {
        jobject jmap = JavaMakeMap( context->env, params );

        jmethodID mid = context->env->GetStaticMethodID(context->jClass, functionName.c_str(), "(Ljava/util/HashMap;)V");
        if (mid == nullptr) {
            std::cout << "could not get method : " << functionName << std::endl;
            return;
        }

        context->env->CallVoidMethod(context->jObject, mid, jmap );
    }

    void execFunction(
        const std::unique_ptr<JavaContext> &context,
        std::string functionName
    ) {
        jmethodID mid = context->env->GetStaticMethodID(context->jClass, functionName.c_str(), "()V");
        if (mid == nullptr) {
            std::cout << "could not get method : " << functionName << std::endl;
            return;
        }

        context->env->CallVoidMethod(context->jObject, mid);
    }



    JNIEXPORT jobject JNICALL JavaMakeMap(JNIEnv *env, std::unordered_map<std::string,std::string> mMap) {
        env->PushLocalFrame(256); // fix for local references
        jclass hashMapClass= env->FindClass( "java/util/HashMap" );
        jmethodID hashMapInit = env->GetMethodID( hashMapClass, "<init>", "(I)V");
        jobject hashMapObj = env->NewObject( hashMapClass, hashMapInit, mMap.size());
        jmethodID hashMapOut = env->GetMethodID( hashMapClass, "put",
          "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        for (auto it : mMap)
        {
            env->CallObjectMethod( hashMapObj, hashMapOut,
                 env->NewStringUTF(it.first.c_str()),
                 env->NewStringUTF(it.second.c_str()));
        }

        env->PopLocalFrame(hashMapObj);
        return hashMapObj;
    }

};
