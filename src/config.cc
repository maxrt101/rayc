#include <rayc/config.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <rayc/utils/string.h>
#include <rayc/exception.h>


rayc::Config::ParsingError::ParsingError(const std::string& msg)
  : std::runtime_error("Config Parsing Error: " + msg) {}

bool rayc::Config::exists(const std::string& key) const {
  return mData.find(key) != mData.end();
}

bool rayc::Config::ifExists(const std::string& path, std::function<void(const std::string&)> cb) const {
  if (exists(path)) {
    cb(getValue(path));
    return true;
  }
  return false;
}


std::string rayc::Config::getValue(const std::string& key) const {
  return mData[key];
}

std::string rayc::Config::getValueOr(const std::string& path, const std::string& defaultValue) const {
  if (exists(path)) {
    return getValue(path);
  }
  return defaultValue;
}

void rayc::Config::setValue(const std::string& key, const std::string& value) {
  mData[key] = value;
}

std::vector<std::string> rayc::Config::keys() const {
  std::vector<std::string> result;

  for (const auto& [key, _] : mData) {
    result.push_back(key);
  }

  return result;
}

std::string& rayc::Config::operator[](const std::string& key) {
  return mData[key];
}

std::string rayc::Config::toString() const {
  std::stringstream ss;
  for (auto& [key, value] : mData) {
    ss << key << "=" << value << "\n";
  }
  return ss.str();
}

void rayc::Config::toFile(const std::string& filename) {
  std::ofstream file(filename);
  if (!file) {
    throw FileOpenException(filename);
  }
  file << toString();
}

rayc::Config rayc::Config::fromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
    throw FileOpenException(filename);
  }
  std::stringstream ss;
  ss << file.rdbuf();
  return Config::fromString(ss.str());
}

rayc::Config rayc::Config::fromString(const std::string& src) {
  std::vector<std::string> lines = strsplit(src, "\n");
  std::vector<std::string> sectionStack;
  Config config;

  int lineNumber = 0;
  for (auto& line : lines) {
    lineNumber++;
    if (!line.size() || line.front() == '#') continue;

    size_t i = line.find("=");
    if (i != std::string::npos) {
      std::string key = strstrip(line.substr(0, i)), value = strstrip(line.substr(i+1));
      config.setValue(key, value);
    } else {
      throw ParsingError(strfmt("Exected a key-value pair (at '{}' line {})", line, lineNumber));
    }
  }

  return config;
}
