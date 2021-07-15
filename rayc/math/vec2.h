#ifndef _RAYC_MATH_VEC2_H
#define _RAYC_MATH_VEC2_H

namespace rayc {

template <typename T>
struct Vec2 {
  T x;
  T y;

  inline Vec2() : x(0), y(0) {}
  inline Vec2(T x, T y) : x(x), y(y) {}

  inline Vec2<T>& operator=(const Vec2<T>& rhs) {
    x = rhs.x;
    y = rhs.y;
    return *this;
  }

  inline bool operator==(const Vec2<T>& rhs) const { return x == rhs.x && y == rhs.y; }
  inline bool operator!=(const Vec2<T>& rhs) const { return x != rhs.x || y != rhs.y; }
};

typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2u;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;

} // namespace rayc

#endif
