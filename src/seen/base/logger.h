// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

// NOTE(Autokaka): Include the following headers by design!

#include <fmt/core.h>
#include <string>

#include "seen/pal/pal.h"

namespace seen {

enum class LogLevel { kDebug, kInfo, kWarn, kError, kFatal };
#define SEEN_LOG(Level, FileName, Line, ...)                                                                    \
  {                                                                                                             \
    using namespace std::string_literals;                                                                       \
    pal::log(static_cast<int>(seen::LogLevel::Level),                                                           \
             ("@SeenKit["s + FileName + ":" + std::to_string(Line) + "] " + fmt::format(__VA_ARGS__)).c_str()); \
    if constexpr (seen::LogLevel::Level >= seen::LogLevel::kFatal) {                                            \
      abort();                                                                                                  \
    }                                                                                                           \
  }

#if SEEN_ENABLE_DEBUG_LOG
#define SEEN_DEBUG(...) SEEN_LOG(kDebug, __FILE_NAME__, __LINE__, __VA_ARGS__)
#else
#define SEEN_DEBUG(...)
#endif

#define SEEN_INFO(...) SEEN_LOG(kInfo, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_WARN(...) SEEN_LOG(kWarn, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_ERROR(...) SEEN_LOG(kError, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_FATAL(...) SEEN_LOG(kFatal, __FILE_NAME__, __LINE__, __VA_ARGS__)

// clang-format off
#define SEEN_ASSERT_WITH_MESSAGE(Condition, ...) if (Condition) {} else do SEEN_FATAL(__VA_ARGS__) while (false)
#define SEEN_ASSERT(Condition) if (Condition) {} else do SEEN_FATAL("Assert {} failed.", #Condition) while (false)
// clang-format on

}  // namespace seen
