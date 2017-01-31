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
#include "../../../util/logger.hpp"

namespace smart_contract {

    void Java_SmartContract_save(JNIEnv *env, jobject thiz, jstring key, jstring value) {
        const char *keyChar = env->GetStringUTFChars(key, 0);
        const char *valueChar = env->GetStringUTFChars(value, 0);

        std::cout << "[" << keyChar << ", " << valueChar << std::endl;

        env->ReleaseStringUTFChars(key, keyChar);
        env->ReleaseStringUTFChars(value, valueChar);
    }

    std::unique_ptr<JavaContext> initializeVM(const std::string& packageName, const std::string& contractName) {

        const auto IrohaHome = []() {
            const auto p = getenv("IROHA_HOME");
            if (p == nullptr) {
                std::cout << "You must set IROHA_HOME!" << std::endl;
                return std::string();
            }
            return std::string(p);
        }();

        // paths are hard coding here...
        std::vector<std::string> java_args = {
            "-Djava.class.path="   + IrohaHome + "/smart_contract",
            "-Djava.library.path=" + IrohaHome + "/build/lib",
            "-Djava.security.policy=" + IrohaHome + "/core/infra/smart_contract/jvm/java.policy.txt",
            "-Djava.security.manager",
        };

        const int OptionSize = java_args.size();

        JavaVMOption options[OptionSize];
        for (int i=0; i<OptionSize; i++) {
            options[i].optionString = const_cast<char*>( java_args[i].c_str() );
        }
        
        {
            for (int i=0; i<OptionSize; i++) {
                std::cout << options[i].optionString << " ";
            }
            std::cout << packageName + "." + contractName << std::endl;
        }
        
        JavaVMInitArgs vm_args;
        vm_args.version  = JNI_VERSION_1_6;
        vm_args.options  = options;
        vm_args.nOptions = OptionSize;
        vm_args.ignoreUnrecognized = JNI_FALSE;

        JNIEnv *env;
        JavaVM *jvm;

        int res = JNI_CreateJavaVM(&jvm, (void **) &env, &vm_args);
        if (res) {
            std::cout << "cannot run JavaVM : " << res << std::endl;
            return nullptr;
        }

//        std::string package_name = contractName;
//        std::transform(package_name.begin(), package_name.end(), package_name.begin(), ::tolower);
        auto slashPackageName = packageName;
        std::transform(slashPackageName.begin(), slashPackageName.end(), slashPackageName.begin(), [](const char a) {
            return a == '.' ? '/' : a;
        });

        jclass cls = env->FindClass( (slashPackageName + "/" + contractName).c_str() );
        if (cls == nullptr) {
            std::cout << "could not found class : " << packageName << "." << contractName << std::endl;
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
            packageName + "." + contractName,
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
