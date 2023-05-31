#include <rayc/version.h>
#include <rayc/utils/string.h>

static constexpr uint8_t major = 0, minor = 1, patch = 0;

std::string rayc::getVersionString() {
  return strfmt("{}.{}.{}", major, minor, patch);
}

rayc::Version rayc::getVersion() {
  return Version {
    .major = major,
    .minor = minor,
    .patch = patch
  };
}

uint32_t rayc::getVersionCode() {
  return (major << 16) & (minor << 8) & patch;
}
