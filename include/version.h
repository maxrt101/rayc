#ifndef _RAYC_VERSION_H_
#define _RAYC_VERSION_H_ 1

#include <cstdint>
#include <string>

namespace rayc {

constexpr uint32_t SERIALIZER_VERSION = 0x1;

struct Version {
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
};

std::string getVersionString();
Version getVersion();
uint32_t getVersionCode();

} /* namespace rayc */

#endif /* _RAYC_VERSION_H_ */