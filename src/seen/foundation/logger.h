// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/16.

#pragma once

#include <fmt/core.h>
#include <memory>
#include <string>

namespace seen {

class CFLog {
 public:
  enum class Level { kInfo, kWarn, kError, kFatal };

  template <typename... Args>
  static void Print(const Level& level, const char* file_name, int line, const char* fmt = "", Args... args) {
    using namespace std::string_literals;
    GetInstance()(level, "@SeenKit["s.append(file_name)
                             .append(":")
                             .append(std::to_string(line))
                             .append("] ")
                             .append(fmt::format(fmt, args...)));
    if (level >= Level::kFatal) {
      abort();
    }
  }

 protected:
  static std::shared_ptr<CFLog> GetInstance();
  virtual void operator()(const Level& level, const std::string& message) = 0;
};

#define SEEN_INFO(...) seen::CFLog::Print(seen::CFLog::Level::kInfo, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_WARN(...) seen::CFLog::Print(seen::CFLog::Level::kWarn, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_ERROR(...) seen::CFLog::Print(seen::CFLog::Level::kError, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_FATAL(...) seen::CFLog::Print(seen::CFLog::Level::kFatal, __FILE_NAME__, __LINE__, __VA_ARGS__)

#define SEEN_ASSERT_WITH_MESSAGE(Condition, ...) (Condition) ? static_cast<void>(0) : SEEN_LOG_FATAL(__VA_ARGS__)
#define SEEN_ASSERT(Condition) (Condition) ? static_cast<void>(0) : SEEN_FATAL("Assert `%s` failed.", #Condition)

}  // namespace seen
