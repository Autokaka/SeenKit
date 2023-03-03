/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/16.
 */

#include "logger.h"

namespace seen::CFLog {

std::array<Delegate, kLogLevelCount> gDelegates;
std::mutex gDelegatesMutex;

void SetDelegate(const Level& level, const Delegate& delegate) {
  std::scoped_lock lock(gDelegatesMutex);
  gDelegates[static_cast<int>(level)] = delegate;
}

}  // namespace seen::CFLog
