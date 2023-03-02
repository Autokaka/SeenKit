/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include "./engine.h"

namespace seen {

Engine::Engine() {}

CFPromise<bool> Engine::RunModule(const std::vector<std::byte>& module_data) {}

CFPromise<void> Engine::Draw(double timeDeltaMillisec) {}

CFPromise<void> Engine::Reset() {}

}  // namespace seen
