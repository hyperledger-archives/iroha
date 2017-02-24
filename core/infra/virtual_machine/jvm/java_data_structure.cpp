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

#include "java_data_structure.hpp"
#include <map>

namespace virtual_machine {
namespace jvm {

jobject JavaMakeBoolean(JNIEnv *env, jboolean value) {
  jclass booleanClass = env->FindClass("java/lang/Boolean");
  jmethodID methodID = env->GetMethodID(booleanClass, "<init>", "(Z)V");
  return env->NewObject(booleanClass, methodID, value);
}

// HashMap<String, String>
JNIEXPORT jobject JNICALL JavaMakeMap(JNIEnv *env,
                                      std::map<std::string, std::string> mMap) {
  env->PushLocalFrame(256); // fix for local references
  jclass hashMapClass = env->FindClass("java/util/HashMap");
  jmethodID hashMapInit = env->GetMethodID(hashMapClass, "<init>", "(I)V");
  jobject hashMapObj = env->NewObject(hashMapClass, hashMapInit, mMap.size());
  jmethodID hashMapOut = env->GetMethodID(
      hashMapClass, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

  for (auto it : mMap) {
    env->CallObjectMethod(hashMapObj, hashMapOut,
                          env->NewStringUTF(it.first.c_str()),
                          env->NewStringUTF(it.second.c_str()));
  }

  env->PopLocalFrame(hashMapObj);
  return hashMapObj;
}

// HashMap<String, HashMap<String, String>>
JNIEXPORT jobject JNICALL
JavaMakeMap(JNIEnv *env,
            std::map<std::string, std::map<std::string, std::string>> mMap) {
  env->PushLocalFrame(256); // fix for local references
  jclass hashMapClass = env->FindClass("java/util/HashMap");
  jmethodID hashMapInit = env->GetMethodID(hashMapClass, "<init>", "(I)V");
  jobject hashMapObj = env->NewObject(hashMapClass, hashMapInit, mMap.size());
  jmethodID hashMapOut = env->GetMethodID(
      hashMapClass, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

  for (auto it : mMap) {
    env->CallObjectMethod(hashMapObj, hashMapOut,
                          env->NewStringUTF(it.first.c_str()),
                          JavaMakeMap(env, it.second));
  }

  env->PopLocalFrame(hashMapObj);
  return hashMapObj;
}

std::vector<std::string>
convertJavaStringArrayRelease(JNIEnv *env, jobjectArray javaArray_) {
  std::vector<std::string> ret;
  const int arraySize = env->GetArrayLength(javaArray_);
  for (int i = 0; i < arraySize; i++) {
    jstring elementString_ =
        (jstring)(env->GetObjectArrayElement(javaArray_, i));
    const char *elementCString = env->GetStringUTFChars(elementString_, 0);
    ret.push_back(std::string(elementCString));
    env->ReleaseStringUTFChars(elementString_, elementCString);
  }
  return ret;
}

JNIEXPORT jobjectArray JNICALL
JavaMakeStringArray(JNIEnv *env, const std::vector<std::string> &vec) {
  jobjectArray ret;
  ret = (jobjectArray)env->NewObjectArray(
      vec.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
  for (std::size_t i = 0; i < vec.size(); i++)
    env->SetObjectArrayElement(ret, i, env->NewStringUTF(vec[i].c_str()));
  return ret;
}

std::map<std::string, std::string>
convertJavaHashMapValueString(JNIEnv *env, jobject hashMapObj_) {
  /*
    Map<String, String> map = ...
    for (Map.Entry<String, String> entry : map.entrySet())
    {
        ...
    }
   */
  // ref:
  // https://android.googlesource.com/platform/frameworks/base.git/+/a3804cf77f0edd93f6247a055cdafb856b117eec/media/jni/android_media_MediaMetadataRetriever.cpp
  jclass mapClass = env->FindClass("java/util/Map");
  jmethodID entrySet =
      env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
  jobject set = env->CallObjectMethod(hashMapObj_, entrySet);
  jclass setClass = env->FindClass("java/util/Set");
  jmethodID iterator =
      env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
  jobject iter = env->CallObjectMethod(set, iterator);
  jclass iteratorClass = env->FindClass("java/util/Iterator");
  jmethodID hasNext = env->GetMethodID(iteratorClass, "hasNext", "()Z");
  jmethodID next =
      env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
  jclass entryClass = env->FindClass("java/util/Map$Entry");
  jmethodID getKey =
      env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
  jmethodID getValue =
      env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

  std::map<std::string, std::string> ret;

  while (env->CallBooleanMethod(iter, hasNext)) {
    jobject entry = env->CallObjectMethod(iter, next);
    jstring key = (jstring)env->CallObjectMethod(entry, getKey);

    const char *keyStr = env->GetStringUTFChars(key, nullptr);
    if (!keyStr) { // Out of memory
      return {};
    }

    jstring value = (jstring)env->CallObjectMethod(entry, getValue);
    const char *valueStr = env->GetStringUTFChars(value, nullptr);
    if (!valueStr) { // Out of memory
      env->ReleaseStringUTFChars(key, keyStr);
      return {};
    }

    ret[std::string(keyStr)] = std::string(valueStr);

    env->ReleaseStringUTFChars(value, valueStr);
    env->DeleteLocalRef(value);

    env->DeleteLocalRef(entry);
    env->ReleaseStringUTFChars(key, keyStr);
    env->DeleteLocalRef(key);
  }
  return ret;
}

std::map<std::string, std::map<std::string, std::string>>
convertJavaHashMapValueHashMap(JNIEnv *env, jobject hashMapObj_) {
  /*
    Map<String, String> map = ...
    for (Map.Entry<String, String> entry : map.entrySet())
    {
        ...
    }
   */
  // ref:
  // https://android.googlesource.com/platform/frameworks/base.git/+/a3804cf77f0edd93f6247a055cdafb856b117eec/media/jni/android_media_MediaMetadataRetriever.cpp
  jclass mapClass = env->FindClass("java/util/Map");
  jmethodID entrySet =
      env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
  jobject set = env->CallObjectMethod(hashMapObj_, entrySet);
  jclass setClass = env->FindClass("java/util/Set");
  jmethodID iterator =
      env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
  jobject iter = env->CallObjectMethod(set, iterator);
  jclass iteratorClass = env->FindClass("java/util/Iterator");
  jmethodID hasNext = env->GetMethodID(iteratorClass, "hasNext", "()Z");
  jmethodID next =
      env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
  jclass entryClass = env->FindClass("java/util/Map$Entry");
  jmethodID getKey =
      env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
  jmethodID getValue =
      env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

  std::map<std::string, std::map<std::string, std::string>> ret;

  while (env->CallBooleanMethod(iter, hasNext)) {
    jobject entry = env->CallObjectMethod(iter, next);
    jstring key = (jstring)env->CallObjectMethod(entry, getKey);

    const char *keyStr = env->GetStringUTFChars(key, nullptr);
    if (!keyStr) { // Out of memory
      return {};
    }

    jobject value = (jobject)env->CallObjectMethod(entry, getValue);
    ret[std::string(keyStr)] = convertJavaHashMapValueString(env, value);

    env->DeleteLocalRef(entry);
    env->ReleaseStringUTFChars(key, keyStr);
    env->DeleteLocalRef(key);
  }
  return ret;
}
}
}