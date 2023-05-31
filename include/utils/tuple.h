#ifndef _RAYC_UTILS_TUPLE_H_
#define _RAYC_UTILS_TUPLE_H_ 1

#include <tuple>

namespace rayc {

template <class Tuple, class Fn, std::size_t... I>
constexpr void tuple_for_each_impl(Tuple& t, Fn& f, std::index_sequence<I...>) {
  (f(std::get<I>(std::forward<Tuple>(t))), ...);
}

template <class Tuple, class Fn>
constexpr void tuple_for_each(Tuple& t, Fn& f) {
  tuple_for_each_impl(t, f,
    std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{}
  );
}

template<class Struct, class Tuple, std::size_t... I>
constexpr Struct tuple_to_struct_impl(Tuple&& t, std::index_sequence<I...>) {
  return {std::get<I>(std::forward<Tuple>(t))...};
}

template<class Struct, class Tuple>
constexpr Struct tuple_to_struct(Tuple&& t) {
  return tuple_to_struct_impl<Struct>(
    std::forward<Tuple>(t),
    std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{}
  );
}

template<class T, class... Args>
bool isIn(T&& value, Args&&... args) {
  return ((value == args) || ...);
}

} /* namespace rayc */

#endif /* _RAYC_UTILS_TUPLE_H_ */