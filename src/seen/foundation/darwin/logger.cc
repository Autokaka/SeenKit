// Created by Autokaka (qq1909698494@gmail.com) on 2023/07/30.

#if SEEN_BUILD_DARWIN

#include <sys/syslog.h>

#include "seen/foundation/darwin/logger.h"

namespace seen {

void CFLogDarwin::operator()(const Level& level, const std::string& message) {
  switch (level) {
    case Level::kInfo:
      return syslog(LOG_INFO, "%s", message.c_str());
    case Level::kWarn:
      return syslog(LOG_WARNING, "%s", message.c_str());
    case Level::kError:
      return syslog(LOG_ERR, "%s", message.c_str());
    case Level::kFatal:
      return syslog(LOG_CRIT, "%s", message.c_str());
  }
}

}  // namespace seen

#endif  // SEEN_BUILD_DARWIN
