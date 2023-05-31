#ifndef _RAYC_UTILS_STRING_H_
#define _RAYC_UTILS_STRING_H_ 1

#include <string>
#include <vector>

namespace rayc {

std::vector<std::string> strsplit(const std::string& str, const std::string& delimiter = " ");
std::string strstrip(const std::string& str, char c = ' ');

bool stoi(const std::string& str, int& result);
bool stof(const std::string& str, float& result);

namespace impl {

template <typename T>
inline std::string toString(T value) {
  return std::to_string(value);
}

template <>
inline std::string toString(std::string value) {
  return value;
}

template <>
inline std::string toString(const char* value) {
  return std::string(value);
}

template <typename... Args>
inline void format(const char* fmt, std::string& result) {
  result += fmt;
}

template <typename T, typename... Args>
inline void format(const char* fmt, std::string& result, T& value, Args&&... args) {
  for (size_t i = 0; fmt[i]; i++) {
    if (fmt[i] == '{') {
      std::string fmtString;
      i += 1;
      while (fmt[i] && fmt[i] != '}') fmtString += fmt[i++];
      // TODO: formats
      result += toString(value);
      impl::format(fmt+i+1, result, args...);
      return;
    } else {
      result += fmt[i];
    }
  }
}

} /* namespace impl */

template <typename... Args>
inline std::string strfmt(const std::string& fmt, Args&&... args) {
  std::string result;
  impl::format(fmt.c_str(), result, args...);
  return result;
}

} /* namespace rayc */

#endif /* _RAYC_UTILS_STRING_H_ */