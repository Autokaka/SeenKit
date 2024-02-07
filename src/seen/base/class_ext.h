// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/15.

#pragma once

namespace seen {

#define SEEN_DISALLOW_COPY(TypeName) TypeName(const TypeName&) = delete

#define SEEN_DISALLOW_ASSIGN(TypeName) TypeName& operator=(const TypeName&) = delete

#define SEEN_DISALLOW_MOVE(TypeName) \
  TypeName(TypeName&&) = delete;     \
  TypeName& operator=(TypeName&&) = delete

#define SEEN_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;           \
  TypeName& operator=(const TypeName&) = delete

#define SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(TypeName) \
  TypeName(const TypeName&) = delete;                \
  TypeName(TypeName&&) = delete;                     \
  TypeName& operator=(const TypeName&) = delete;     \
  TypeName& operator=(TypeName&&) = delete

#define SEEN_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                                \
  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(TypeName)

}  // namespace seen
