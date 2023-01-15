/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/16.
 */

#pragma once

#include <functional>
#include <mutex>
#include <string>

namespace seen::CFLog {

constexpr int kLogLevelCount = 4;

enum class Level { kInfo, kWarn, kError, kFatal };

using Delegate = std::function<void(const Level& level, const char* file_name, int line, const char* msg)>;
Delegate gDelegates[kLogLevelCount] = {nullptr};
std::mutex gDelegatesMutex;

void Print(const std::string& message);

void SetDelegate(const Level& level, const Delegate& delegate) {
  std::scoped_lock lock(gDelegatesMutex);
  gDelegates[static_cast<int>(level)] = delegate;
}

template <typename... Args>
void Print(const std::string& tag, const Level& level, const char* file, int line, const char* fmt = "", Args... args) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
  using namespace std::string_literals;
  int size_s = std::snprintf(nullptr, 0, fmt, args...) + 1;
  auto size = static_cast<size_t>(size_s);
  auto buf = std::make_unique<char[]>(size);
  size_s = std::snprintf(buf.get(), size, fmt, args...);
  std::string fmt_msg(buf.get(), buf.get() + size - 1);
#pragma clang diagnostic pop

  std::scoped_lock lock(gDelegatesMutex);
  if (auto delegate = gDelegates[static_cast<int>(level)]) {
    delegate(level, file, line, fmt_msg.data());
  } else {
    auto msg = "@SeenKit["s + file + ":" + std::to_string(line) + "]<" + tag + "> " + fmt_msg;
    Print(msg);
  }
  if (level >= Level::kFatal) {
    abort();
  }
}

#define SEEN_INFO(...) seen::CFLog::Print("INFO", seen::CFLog::Level::kInfo, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_WARN(...) seen::CFLog::Print("WARN", seen::CFLog::Level::kWarn, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_ERROR(...) seen::CFLog::Print("ERROR", seen::CFLog::Level::kError, __FILE_NAME__, __LINE__, __VA_ARGS__)
#define SEEN_FATAL(...) seen::CFLog::Print("FATAL", seen::CFLog::Level::kFatal, __FILE_NAME__, __LINE__, __VA_ARGS__)

#define SEEN_ASSERT_WITH_MESSAGE(Condition, ...) Condition ? static_cast<void>(0) : SEEN_LOG_FATAL(__VA_ARGS__)
#define SEEN_ASSERT(Condition) Condition ? static_cast<void>(0) : SEEN_LOG_FATAL("Assert `%s` failed.", #Condition)

}  // namespace seen::CFLog
