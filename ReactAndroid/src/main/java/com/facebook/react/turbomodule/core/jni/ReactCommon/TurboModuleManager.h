/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <ReactCommon/CallInvokerHolder.h>
#include <ReactCommon/JavaTurboModule.h>
#include <ReactCommon/LongLivedObject.h>
#include <ReactCommon/RuntimeExecutor.h>
#include <ReactCommon/TurboModule.h>
#include <ReactCommon/TurboModuleManagerDelegate.h>
#include <fbjni/fbjni.h>
#include <jsi/jsi.h>
#include <react/jni/CxxModuleWrapper.h>
#include <react/jni/JMessageQueueThread.h>
#include <react/jni/JRuntimeExecutor.h>
#include <memory>
#include <unordered_map>

namespace facebook {
namespace react {

class TurboModuleManager : public jni::HybridClass<TurboModuleManager> {
 public:
  static auto constexpr kJavaDescriptor =
      "Lcom/facebook/react/turbomodule/core/TurboModuleManager;";
  static jni::local_ref<jhybriddata> initHybrid(
      jni::alias_ref<jhybridobject> jThis,
      jni::alias_ref<JRuntimeExecutor::javaobject> runtimeExecutor,
      jni::alias_ref<CallInvokerHolder::javaobject> jsCallInvokerHolder,
      jni::alias_ref<CallInvokerHolder::javaobject> nativeCallInvokerHolder,
      jni::alias_ref<TurboModuleManagerDelegate::javaobject> delegate,
      bool useGlobalCallbackCleanupScopeUsingRetainJSCallback,
      bool useTurboModuleManagerCallbackCleanupScope);
  static void registerNatives();

 private:
  friend HybridBase;
  jni::global_ref<TurboModuleManager::javaobject> javaPart_;
  RuntimeExecutor runtimeExecutor_;
  std::shared_ptr<CallInvoker> jsCallInvoker_;
  std::shared_ptr<CallInvoker> nativeCallInvoker_;
  jni::global_ref<TurboModuleManagerDelegate::javaobject> delegate_;

  JSCallbackRetainer retainJSCallback_;
  std::shared_ptr<LongLivedObjectCollection> longLivedObjectCollection_;

  using TurboModuleCache =
      std::unordered_map<std::string, std::shared_ptr<react::TurboModule>>;

  /**
   * TODO(T48018690):
   * All modules are currently long-lived.
   * We need to come up with a mechanism to allow modules to specify whether
   * they want to be long-lived or short-lived.
   */
  std::shared_ptr<TurboModuleCache> turboModuleCache_;

  void installJSIBindings();
  explicit TurboModuleManager(
      jni::alias_ref<TurboModuleManager::jhybridobject> jThis,
      RuntimeExecutor runtimeExecutor,
      std::shared_ptr<CallInvoker> jsCallInvoker,
      std::shared_ptr<CallInvoker> nativeCallInvoker,
      jni::alias_ref<TurboModuleManagerDelegate::javaobject> delegate,
      bool useGlobalCallbackCleanupScopeUsingRetainJSCallback,
      bool useTurboModuleManagerCallbackCleanupScope);
};

} // namespace react
} // namespace facebook
