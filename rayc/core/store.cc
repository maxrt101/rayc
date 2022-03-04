#include "core/store.h"

#include <exception>

rayc::store::String* rayc::store::Value::asString() {
  String* strPtr = dynamic_cast<String*>(this);
  if (!strPtr) {
    throw std::bad_cast();
  }
  return strPtr;
}

rayc::store::Integer* rayc::store::Value::asInteger() {
  Integer* intPtr = dynamic_cast<Integer*>(this);
  if (!intPtr) {
    throw std::bad_cast();
  }
  return intPtr;
}

rayc::store::Float* rayc::store::Value::asFloat() {
  Float* floatPtr = dynamic_cast<Float*>(this);
  if (!floatPtr) {
    throw std::bad_cast();
  }
  return floatPtr;
}

rayc::store::VoidPtr* rayc::store::Value::asVoidPtr() {
  VoidPtr* voidPtr = dynamic_cast<VoidPtr*>(this);
  if (!voidPtr) {
    throw std::bad_cast();
  }
  return voidPtr;
}

rayc::store::String::String(const std::string& data)
  : m_data(data) {}

std::string rayc::store::String::get() const {
  return m_data;
}

void rayc::store::String::set(const std::string& data) {
  m_data = data;
}

rayc::store::Integer::Integer(int data)
  : m_data(data) {}

int rayc::store::Integer::get() const {
  return m_data;
}

void rayc::store::Integer::set(int data) {
  m_data = data;
}

rayc::store::Float::Float(float data)
  : m_data(data) {}

float rayc::store::Float::get() const {
  return m_data;
}

void rayc::store::Float::set(float data) {
  m_data = data;
}

rayc::store::VoidPtr::VoidPtr(void* data)
  : m_data(data) {}

void* rayc::store::VoidPtr::get() const {
  return m_data;
}

void rayc::store::VoidPtr::set(void* data) {
  m_data = data;
}
