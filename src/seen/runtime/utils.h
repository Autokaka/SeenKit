// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#pragma once

namespace seen::runtime::utils {

template <auto OrigFn>
struct CFunction {
  template <typename ClassType, typename ReturnType, typename... Args>
  static ReturnType Apply(const ClassType* caller, Args... args) {
    return (caller->*OrigFn)(args...);
  }

  template <typename ClassType, typename ReturnType, typename... Args>
  static ReturnType Apply(ClassType* caller, Args... args) {
    return (caller->*OrigFn)(args...);
  }

  template <typename ReturnType, typename... Args>
  static ReturnType Apply(Args... args) {
    return OrigFn(args...);
  }
};

}  // namespace seen::runtime::utils
