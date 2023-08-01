// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/02.

#pragma once

#include <memory>
#include <string>

#include "seen/foundation/class_constraints.h"
#include "seen/foundation/data.h"
#include "seen/foundation/worker.h"

namespace seen {

class App final {
 public:
  // NOLINTNEXTLINE
  DISALLOW_COPY_AND_ASSIGN(App);

  static void CreateFromData();

  explicit App();
  ~App();
};

class AppSession final {
 public:
  DISALLOW_COPY_ASSIGN_AND_MOVE(AppSession);
  using Token = std::string;
  using MessageHandler = void (*)(const std::string& message, const CFData::Ptr& data);

  void SendMessage(const std::string& message, const CFData::Ptr& data = nullptr);
  void SetMessageHandler(MessageHandler message_handler);

 private:
  CFWorker::Ptr worker_ = nullptr;
  MessageHandler message_handler_ = nullptr;
};

}  // namespace seen
