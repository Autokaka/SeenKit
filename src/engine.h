/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include "foundation/promise.h"

namespace seen {

class Engine final {
 public:
  Engine();

  CFPromise<bool> RunModule(const std::vector<std::byte>& module_data);

  CFPromise<void> Draw(double timeDeltaMillisec);

  CFPromise<void> Reset();
};

}  // namespace seen
