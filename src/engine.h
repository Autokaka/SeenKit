/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include <functional>

namespace seen {

class Engine {
 public:
  Engine();

  void RunModule(const std::vector<std::byte>& module_data, const std::function<void(bool)>& on_complete = nullptr);

  void Draw(double timeDeltaMillisec, const std::function<void()>& on_complete = nullptr);

  void Reset(const std::function<void()>& on_complete = nullptr);
};

}  // namespace seen
