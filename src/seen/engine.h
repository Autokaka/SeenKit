// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.

#pragma once

#include <memory>

#include "seen/base/class_ext.h"
#include "seen/base/data_channel.h"
#include "seen/base/worker.h"
#include "seen/bundle.h"
#include "seen/mod/seen.h"
#include "seen/runtime/entry.h"

namespace seen {

class Engine final : public std::enable_shared_from_this<Engine> {
 public:
  using Ptr = std::shared_ptr<Engine>;
  using CreateCallback = std::function<void(const Ptr& engine)>;
  using InitCallback = std::function<void(bool success)>;

  static void CreateAsync(const Bundle::Ptr& bundle, CreateCallback callback);
  static Ptr Create(const Bundle::Ptr& bundle);

  explicit Engine();
  ~Engine();

  void IsRunning(bool is_running);
  [[nodiscard]] bool IsRunning() const;
  void Drawable(const void* drawable);
  const void* Drawable() const;
  void DrawableMetrics(const mod::DrawableMetrics& metrics);
  mod::DrawableMetrics DrawableMetrics() const;

 private:
  void MainInit(const Bundle::Ptr& bundle, InitCallback callback);
  void MainRelease();
  mod::Seen::Ptr GetSeen() const;

  CFWorker::Ptr main_worker_;
  CFDataChannel::Ptr main_channel_;
  CFDataChannel::Ptr platform_channel_;
  runtime::StatePtr state_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Engine);
};

}  // namespace seen
