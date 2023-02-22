/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/16.
 */

#import <Foundation/Foundation.h>

#include <syslog.h>
#include <string>

namespace seen::CFLog {

void CFLogPrint(const std::string& message) {
  syslog(LOG_ALERT, "%s", message.c_str());
}

}  // namespace seen::CFLog
