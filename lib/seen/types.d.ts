// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

type Tuple<T, N extends number> = N extends N ? (number extends N ? T[] : _TupleOf<T, N, []>) : never;
type _TupleOf<T, N extends number, R extends unknown[]> = R["length"] extends N ? R : _TupleOf<T, N, [T, ...R]>;

declare namespace Seen {
  type Vec<N extends number> = Readonly<Tuple<number, N>>;
  type Mat<N extends number> = Readonly<Tuple<Tuple<number, N>, N>>;
}
