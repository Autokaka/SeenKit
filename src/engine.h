/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

namespace seen {

class Engine {
 public:
  explicit Engine();

  void Play(const std::function<void()>& on_complete = nullptr);
  void Pause(const std::function<void()>& on_complete = nullptr);
  bool IsPaused();

  void RunModule(const std::vector<std::byte>& module_data, const std::function<void()>& on_complete = nullptr);

  void Draw(double timeDeltaMillisec, const std::function<void()>& on_complete = nullptr);

  void Reset(const std::function<void()>& on_complete = nullptr);
};

}  // namespace seen
