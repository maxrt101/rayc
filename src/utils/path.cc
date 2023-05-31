#include <rayc/utils/path.h>
#include <rayc/utils/string.h>

std::string rayc::getFileNameFromPath(const std::string& path) {
  std::string name = strstrip(path, ' ');

  while (name.back() == '/') name.pop_back();

  name = strsplit(name, "/").back();

  return name;
}
