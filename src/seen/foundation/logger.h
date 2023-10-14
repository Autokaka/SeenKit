// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/16.

#pragma once

#include <fmt/core.h>
#include <string>

#include "seen/pal/pal.h"

namespace seen {

enum class CFLogLevel { kInfo, kWarn, kError, kFatal };
template <typename... T>
static void CFLog(const CFLogLevel& level,
                  const char* file_name,
                  int line,
                  fmt::format_string<T...> fmt = "",
                  T&&... args) {
  using namespace std::string_literals;
  auto message = "@SeenKit["s.append(file_name)
                     .append(":")
                     .append(std::to_string(line))
                     .append("] ")
                     .append(fmt::format(fmt, args...));
  pal::log(level, message.c_str());
  if (level >= CFLogLevel::kFatal) {
    abort();
  }
}

#define SEEN_INFO(...) seen::CFLog(seen::CFLogLevel::kInfo, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_WARN(...) seen::CFLog(seen::CFLogLevel::kWarn, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_ERROR(...) seen::CFLog(seen::CFLogLevel::kError, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_FATAL(...) seen::CFLog(seen::CFLogLevel::kFatal, __FILE_NAME__, __LINE__, __VA_ARGS__)

#define SEEN_ASSERT_WITH_MESSAGE(Condition, ...) (Condition) ? static_cast<void>(0) : SEEN_FATAL(__VA_ARGS__)
#define SEEN_ASSERT(Condition) (Condition) ? static_cast<void>(0) : SEEN_FATAL("Assert {} failed.", #Condition)

}  // namespace seen
