#ifndef _RAYC_CORE_EVENT_H
#define _RAYC_CORE_EVENT_H

#include <string>

namespace rayc {

class Event {
 public:
  Event(const std::string& name);
  virtual ~Event();

  std::string getName() const;

 private:
  std::string m_name; // maybe replace with some kind of id?
};

class EventListener {
 public:
  virtual void onEvent(const Event& event) = 0;
};

} /* namespace rayc */

#endif