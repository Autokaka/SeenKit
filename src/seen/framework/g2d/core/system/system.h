/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/03/08.
 */

#pragma once

#include <cstddef>
#include <memory>

namespace seen::framework::g2d::core {

struct IOpaqueSystem {};
using IOpaqueSystemPtr = std::shared_ptr<IOpaqueSystem>;

template <typename InitOptions, typename DestroyOptions>
struct ISystem : IOpaqueSystem {
  virtual void Init(InitOptions options) {}
  virtual void Destroy(DestroyOptions options) {}
};

template <typename InitOptions>
struct ISystem<InitOptions, void> {
  virtual void Init(InitOptions options) {}
  virtual void Destroy() {}
};

template <typename DestroyOptions>
struct ISystem<void, DestroyOptions> {
  virtual void Init() {}
  virtual void Destroy(DestroyOptions options) {}
};

template <>
struct ISystem<void, void> {
  virtual void Init() {}
  virtual void Destroy() {}
};

template <typename R /** Renderer */ = std::nullptr_t, typename Ctor = std::nullptr_t, typename Dtor = std::nullptr_t>
using ISystemConstructor = ISystem<Ctor, Dtor> (*)(R);

}  // namespace seen::framework::g2d::core
