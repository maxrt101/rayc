#ifndef _RAYC_VIEW_H_
#define _RAYC_VIEW_H_ 1

#include <map>
#include <vector>
#include <string>
#include <functional>

namespace rayc {

class View {
 public:
  View(const std::string& name);
  virtual ~View();

  std::string getName() const;
  bool isActive() const;
  void setActive(bool isActive);

  virtual void onFrameUpdate(float frameTime);
  virtual void onEvent(const std::string& eventName, std::map<std::string, std::string> args);

 private:
  std::string mName;
  bool mIsActive = true;
};

} /* namespace rayc */

#endif /* _RAYC_VIEW_H_ */