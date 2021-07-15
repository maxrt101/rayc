#include "core/event.h"

rayc::Event::Event(const std::string& name) : m_name(name) {}

rayc::Event::~Event() {}

std::string rayc::Event::getName() const {
  return m_name;
}