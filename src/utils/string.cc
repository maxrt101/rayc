#include <rayc/utils/string.h>
#include <rayc/log.h>

std::vector<std::string> rayc::strsplit(const std::string& str, const std::string& delimiter) {
  std::vector<std::string> result;
  size_t last = 0, next = 0;
  while ((next = str.find(delimiter, last)) != std::string::npos) {
    result.push_back(str.substr(last, next-last));
    last = next + 1;
  }
  result.push_back(str.substr(last));
  return result;
}

std::string rayc::strstrip(const std::string& str, char c) {
  std::string result = str;
  while (result.front() == c) result.erase(result.begin());
  while (result.back() == c) result.pop_back();
  return result;
}

bool rayc::stoi(const std::string& str, int& result) {
  try {
    result = std::stoi(str);
    return true;
  } catch (std::invalid_argument& e) {
    error("Cannot convert '%s' to int: %s", str.c_str(), e.what());
    return false;
  } catch (std::out_of_range& e) {
    error("Cannot convert '%s' to int: %s", str.c_str(), e.what());
    return false;
  }
}

bool rayc::stof(const std::string& str, float& result) {
  try {
    result = std::stod(str);
    return true;
  } catch (std::invalid_argument& e) {
    error("Cannot convert '%s' to int: %s", str.c_str(), e.what());
    return false;
  } catch (std::out_of_range& e) {
    error("Cannot convert '%s' to int: %s", str.c_str(), e.what());
    return false;
  }
}
