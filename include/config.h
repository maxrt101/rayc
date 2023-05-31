#ifndef _RAYC_CONFIG_H_
#define _RAYC_CONFIG_H_ 1

#include <map>
#include <string>
#include <vector>
#include <functional>

namespace rayc {

class Config {
 public:
  using Section = std::map<std::string, std::string>;

  struct ParsingError : public std::runtime_error {
    ParsingError(const std::string& msg);
  };

 public:
  Config() = default;
  ~Config() = default;

  bool exists(const std::string& key) const;
  bool ifExists(const std::string& key, std::function<void(const std::string&)> cb) const;

  std::string getValue(const std::string& key) const;
  std::string getValueOr(const std::string& key, const std::string& defaultValue) const;

  void setValue(const std::string& key, const std::string& value);

  std::vector<std::string> keys() const;

  std::string& operator[](const std::string& key);

  std::string toString() const;
  void toFile(const std::string& filename);

  static Config fromFile(const std::string& filename);
  static Config fromString(const std::string& src);

 private:
  mutable std::map<std::string, std::string> mData;
};

} /* namespace rayc */

#endif /* _RAYC_CONFIG_H_ */