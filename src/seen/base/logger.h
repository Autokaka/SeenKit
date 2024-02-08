// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/16.

#pragma once

// include the following headers by design

#include <fmt/core.h>
#include <string>

#include "seen/pal/pal.h"

namespace seen {

enum class CFLogLevel { kInfo, kWarn, kError, kFatal };
#define SEEN_LOG(Level, FileName, Line, ...)                                                                    \
  {                                                                                                             \
    using namespace std::string_literals;                                                                       \
    pal::log(static_cast<int>(seen::CFLogLevel::Level),                                                         \
             ("@SeenKit["s + FileName + ":" + std::to_string(Line) + "] " + fmt::format(__VA_ARGS__)).c_str()); \
    if constexpr (seen::CFLogLevel::Level >= seen::CFLogLevel::kFatal) {                                        \
      abort();                                                                                                  \
    }                                                                                                           \
  }

#define SEEN_INFO(...) SEEN_LOG(kInfo, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_WARN(...) SEEN_LOG(kWarn, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_ERROR(...) SEEN_LOG(kError, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_FATAL(...) SEEN_LOG(kFatal, __FILE_NAME__, __LINE__, __VA_ARGS__)

// clang-format off
#define SEEN_ASSERT_WITH_MESSAGE(Condition, ...) if (Condition) {} else do SEEN_FATAL(__VA_ARGS__) while (false)
#define SEEN_ASSERT(Condition) if (Condition) {} else do SEEN_FATAL("Assert {} failed.", #Condition) while (false)
// clang-format on

}  // namespace seen
