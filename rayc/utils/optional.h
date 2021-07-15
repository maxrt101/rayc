#ifndef _RAYC_UTILS_OPTIONAL_H
#define _RAYC_UTILS_OPTIONAL_H

#include <functional>

namespace rayc {

template <typename T>
class Optional {
 public:
  using value_type = T;
  using function_type = std::function<void(value_type&)>;

 private:
  bool exists_;
  value_type value_;

 public:
  inline Optional() : exists_(false) {}
  inline Optional(value_type value) : exists_(true), value_(value) {}

  bool exists() { return exists_; }
  value_type& get() { return value_; }

  void ifExists(function_type function) {
    if (exists()) {
      function(get());
    }
  }

  void ifNotExists(std::function<void()> function) {
    if (!exists()) {
      function();
    }
  }
};

} /* namespace rayc */

#endif