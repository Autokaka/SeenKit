// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/21.

#if SEEN_BUILD_DARWIN

#pragma once

#import <Foundation/NSObject.h>

#include "seen/foundation/class_ext.h"

@class NSAutoreleasePool;

namespace seen {

template <typename NST>
class scoped_nsprotocol {
 public:
  explicit scoped_nsprotocol(NST object = nil) : object_(object) {}

  scoped_nsprotocol(const scoped_nsprotocol<NST>& that) : object_([that.object_ retain]) {}

  template <typename NSU>
  explicit scoped_nsprotocol(const scoped_nsprotocol<NSU>& that) : object_([that.get() retain]) {}

  ~scoped_nsprotocol() { [object_ release]; }

  scoped_nsprotocol& operator=(const scoped_nsprotocol<NST>& that) {
    if (this == &that) {
      return *this;
    }
    reset([that.get() retain]);
    return *this;
  }

  void reset(NST object = nil) {
    [object_ release];
    object_ = object;
  }

  bool operator==(NST that) const { return object_ == that; }
  bool operator!=(NST that) const { return object_ != that; }

  explicit operator NST() const { return object_; }

  [[nodiscard]] NST get() const { return object_; }

  void swap(scoped_nsprotocol& that) {
    NST temp = that.object_;
    that.object_ = object_;
    object_ = temp;
  }

  NST autorelease() { return [release() autorelease]; }

 private:
  NST object_;

  [[nodiscard]] NST release() {
    NST temp = object_;
    object_ = nil;
    return temp;
  }
};

template <class C>
void swap(scoped_nsprotocol<C>& lhs, scoped_nsprotocol<C>& rhs) {
  lhs.swap(rhs);
}

template <class C>
bool operator==(C lhs, const scoped_nsprotocol<C>& rhs) {
  return lhs == rhs.get();
}

template <class C>
bool operator!=(C lhs, const scoped_nsprotocol<C>& rhs) {
  return lhs != rhs.get();
}

template <typename NST>
class scoped_nsobject : public scoped_nsprotocol<NST*> {
 public:
  explicit scoped_nsobject(NST* object = nil) : scoped_nsprotocol<NST*>(object) {}

  scoped_nsobject(const scoped_nsobject<NST>& that) : scoped_nsprotocol<NST*>(that) {}

  template <typename NSU>
  explicit scoped_nsobject(const scoped_nsobject<NSU>& that) : scoped_nsprotocol<NST*>(that) {}

  scoped_nsobject& operator=(const scoped_nsobject<NST>& that) {
    if (this == &that) {
      return *this;
    }
    scoped_nsprotocol<NST*>::operator=(that);
    return *this;
  }
};

template <>
class scoped_nsobject<id> : public scoped_nsprotocol<id> {
 public:
  explicit scoped_nsobject(id object = nil) : scoped_nsprotocol<id>(object) {}

  scoped_nsobject(const scoped_nsobject<id>& that) = default;

  template <typename NSU>
  explicit scoped_nsobject(const scoped_nsobject<NSU>& that) : scoped_nsprotocol<id>(that) {}

  scoped_nsobject& operator=(const scoped_nsobject<id>& that) {
    if (this == &that) {
      return *this;
    }
    scoped_nsprotocol<id>::operator=(that);
    return *this;
  }
};

template <>
class scoped_nsobject<NSAutoreleasePool> {
 private:
  explicit scoped_nsobject(NSAutoreleasePool* object = nil) {}

  DISALLOW_COPY_ASSIGN_AND_MOVE(scoped_nsobject);
};

}  // namespace seen

#endif
