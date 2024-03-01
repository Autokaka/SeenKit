// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
