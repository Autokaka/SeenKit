// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.

#pragma once

#include "app.h"

namespace seen {

class Engine final : public std::enable_shared_from_this<Engine> {
 public:
  DISALLOW_COPY_ASSIGN_AND_MOVE(Engine);
  using Ptr = std::shared_ptr<Engine>;
  struct Delegate {
    void EngineDidCreateAppSession(const Ptr& engine, const AppSession* app_session);
    void EngineWillDisposeAppSession(const Ptr& engine, const AppSession* app_session);
  };

  explicit Engine();

  void LaunchApp(App&& app);
  void Update(double time_delta_millis);

  [[nodiscard]] std::shared_ptr<Delegate> GetDelegate() const;
  void SetDelegate(const std::shared_ptr<Delegate>& delegate);

 private:
  std::weak_ptr<Delegate> delegate_;
  App app_;
};

}  // namespace seen
