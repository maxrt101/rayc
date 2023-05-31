#ifndef _RAYC_UTILS_ABI_H_
#define _RAYC_UTILS_ABI_H_ 1

#include <cstdlib>
#include <cstdint>

// #include <string>
// #include <memory>
// #include <typeinfo>
// #include <cxxabi.h>

#define RAYC_MULTICHAR4(a, b, c, d) ((((uint32_t)(a)) << 24) | (((uint32_t)(b)) << 16) | (((uint32_t)(c)) << 8) | (d))

namespace rayc {
namespace abi {

union N32 {
  uint8_t  u8[4];
  uint16_t u16[2];
  uint32_t u32;
  int8_t   i8[4];
  int16_t  i16[2];
  int32_t  i32;
  float    f32;
};

void readInt16(N32& result, const uint8_t* data, size_t offset);
void readInt32(N32& result, const uint8_t* data, size_t offset);

void hexdump(const uint8_t* data, size_t length, int cols = 16);

/*inline std::string demangleTypeName(const char* mangled) {
  int status = 0;
  std::unique_ptr<char> result(::abi::__cxa_demangle(mangled, 0, 0, &status));
  if (status) {
    return "<error>";
  }
  return std::string(result.get());
}

template <typename T>
inline std::string getTypeName() {
  return demangleTypeName(typeid(T).name());
}*/

} /* namespace abi */
} /* namespace rayc */

#endif /* _RAYC_UTILS_ABI_H_ */