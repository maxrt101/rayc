#ifndef _RAYC_UI_ALERT_H_
#define _RAYC_UI_ALERT_H_ 1

#include <rayc/view.h>
#include <vector>
#include <string>

namespace rayc {
namespace ui {

class Alert : public View {
 public:
  Alert();
  ~Alert();

  void onFrameUpdate(float frameTime) override;
  void onEvent(const std::string& eventName, std::map<std::string, std::string> args) override;

  void addMessage(float duration, const std::string& msg);
  void clearMessages();

 private:
  struct Message {
    std::string message;
    float duration;
  };

  std::vector<Message> mMessages;
};

} /* namespace rayc */
} /* namespace rayc */

#endif /* _RAYC_UI_ALERT_H_ */