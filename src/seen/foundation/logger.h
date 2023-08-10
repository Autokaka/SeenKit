// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/16.

#pragma once

#include <fmt/core.h>
#include <memory>
#include <string>

namespace seen {

class CFLog {
 public:
  enum class Level { kInfo, kWarn, kError, kFatal };

  explicit CFLog() = default;
  virtual ~CFLog() = default;

  template <typename... T>
  static void Print(const Level& level, const char* fname, int line, fmt::format_string<T...> fmt = "", T&&... args) {
    using namespace std::string_literals;
    auto instance = GetInstance();
    (*instance)(level, "@SeenKit["s.append(fname)
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
#define SEEN_ASSERT(Condition) (Condition) ? static_cast<void>(0) : SEEN_FATAL("Assert {} failed.", #Condition)

}  // namespace seen
