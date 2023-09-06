// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/21.

#if SEEN_BUILD_DARWIN

#import <CoreFoundation/CFBase.h>

namespace seen {

template <class T>
class scoped_cfref {
 public:
  scoped_cfref() : instance_(nullptr) {}

  // NOLINTNEXTLINE(google-explicit-constructor)
  scoped_cfref(T instance) : instance_(instance) {}

  scoped_cfref(const scoped_cfref& other) : instance_(other.instance_) {
    if (instance_) {
      CFRetain(instance_);
    }
  }

  scoped_cfref& operator=(const scoped_cfref&) = delete;

  scoped_cfref(scoped_cfref&& other) noexcept : instance_(other.instance_) { other.instance_ = nullptr; }

  scoped_cfref& operator=(scoped_cfref&& other) noexcept {
    reset(other.release());
    return *this;
  }

  ~scoped_cfref() {
    if (instance_ != nullptr) {
      CFRelease(instance_);
    }
    instance_ = nullptr;
  }

  void reset(T instance = nullptr) {
    if (instance_ != nullptr) {
      CFRelease(instance_);
    }

    instance_ = instance;
  }

  [[nodiscard]] T release() {
    auto instance = instance_;
    instance_ = nullptr;
    return instance;
  }

  // NOLINTNEXTLINE(google-explicit-constructor)
  operator T() const { return instance_; }

  explicit operator bool() const { return instance_ != nullptr; }

 private:
  T instance_;
};

}  // namespace seen

#endif
