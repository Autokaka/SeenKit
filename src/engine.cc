/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include "./engine.h"

namespace seen {

Engine::Engine() : main_looper_(CFCreateLooper()), platform_looper_(CFGetPlatformLooper()) {}

CFPromise<bool> Engine::RunModule(const std::vector<std::byte>& module_data) {
  return CFPromise<bool>([](auto resolve) { resolve(false); });
}

CFPromise<void> Engine::Draw(double timeDeltaMillisec) {
  return CFPromise<void>([](auto resolve) { resolve(); });
}

CFPromise<void> Engine::Reset() {
  return CFPromise<void>([](auto resolve) { resolve(); });
}

}  // namespace seen
