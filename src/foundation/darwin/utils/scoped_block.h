/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/21.
 */

#if SEEN_BUILD_DARWIN

#pragma once

#import <Block.h>

namespace seen {

template <typename B>
class scoped_block {
 public:
  enum class OwnershipPolicy {
    kAssume,
    kRetain,
  };

  explicit scoped_block(B block = nullptr, OwnershipPolicy policy = OwnershipPolicy::kAssume) : block_(block) {
    if (block_ && policy == OwnershipPolicy::kRetain) {
      block_ = Block_copy(block);
    }
  }

  scoped_block(const scoped_block<B>& that) : block_(that.block_) {
    if (block_) {
      block_ = Block_copy(block_);
    }
  }

  ~scoped_block() {
    if (block_) {
      Block_release(block_);
    }
  }

  scoped_block& operator=(const scoped_block<B>& that) {
    if (this == &that) {
      return *this;
    }
    reset(that.get(), OwnershipPolicy::kRetain);
    return *this;
  }

  void reset(B block = nullptr, OwnershipPolicy policy = OwnershipPolicy::kAssume) {
    if (block && policy == OwnershipPolicy::kRetain) {
      block = Block_copy(block);
    }
    if (block_) {
      Block_release(block_);
    }
    block_ = block;
  }

  bool operator==(B that) const { return block_ == that; }

  bool operator!=(B that) const { return block_ != that; }

  explicit operator B() const { return block_; }

  B get() const { return block_; }

  void swap(scoped_block& that) {
    B temp = that.block_;
    that.block_ = block_;
    block_ = temp;
  }

  [[nodiscard]] B release() {
    B temp = block_;
    block_ = nullptr;
    return temp;
  }

 private:
  B block_;
};

}  // namespace seen

#endif
