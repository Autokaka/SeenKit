/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/17.
 */

// This file will list all platform related apis. Developers should implement each api for each platform.

#pragma once

#include <filesystem>
#include <string>

#include "seen/foundation/promise.h"

namespace seen::CFPlatform {

namespace stdfs = std::filesystem;

void CFLogPrint(const std::string& message);

stdfs::path CFFSGetTempDirectory();

CFPromise<void> CFLooperDispatchAsync(const std::function<void()>& task);
CFPromise<void> CFLooperDispatchMicro(const std::function<void()>& task);

}  // namespace seen::CFPlatform
