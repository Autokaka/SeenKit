// Created by Autokaka (qq1909698494@gmail.com) on 2024/03/01.

type Tuple<T, N extends number> = N extends N ? (number extends N ? T[] : _TupleOf<T, N, []>) : never;
type _TupleOf<T, N extends number, R extends unknown[]> = R["length"] extends N ? R : _TupleOf<T, N, [T, ...R]>;

declare namespace Seen {
  type Vec<N extends number> = Readonly<Tuple<number, N>>;
  type Mat<N extends number> = Readonly<Tuple<Tuple<number, N>, N>>;
}
