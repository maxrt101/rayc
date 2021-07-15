#ifndef _RAYC_UTILS_BINARY_H
#define _RAYC_UTILS_BINARY_H

#include <iostream>
#include <fstream>

namespace rayc {

template <typename T>
inline void writeBinary(std::ostream& file, T& data) {
  file.write((char*)&data, sizeof(T));
}

template <typename T>
inline void readBinary(std::istream& file, T* data) {
  file.read((char*)data, sizeof(T));
}

} /* namespace rayc */

#endif