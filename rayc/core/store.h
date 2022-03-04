#ifndef _RAYC_CORE_STORE_H
#define _RAYC_CORE_STORE_H

#include <map>
#include <string>

namespace rayc {
namespace store {

class String;
class Integer;
class Float;
class VoidPtr;

// Input: g_store["inputEnable"] = 
// enum InputState

// Tag?

class Value {
 public:
  Value() = default;
  virtual ~Value() = default;

  String* asString();
  Integer* asInteger();
  Float* asFloat();
  VoidPtr* asVoidPtr();
};

class String : public Value {
 private:
  std::string m_data;

 public:
  String() = default;
  String(const std::string& data);
  ~String() = default;

  std::string get() const;
  void set(const std::string& data);
};

class Integer : public Value {
 private:
  int m_data;

 public:
  Integer() = default;
  Integer(int data);
  ~Integer() = default;

  int get() const;
  void set(int data);
};

class Float : public Value {
 private:
  float m_data;

 public:
  Float() = default;
  Float(float data);
  ~Float() = default;

  float get() const;
  void set(float data);
};

class VoidPtr : public Value {
 private:
  void* m_data;

 public:
  VoidPtr() = default;
  VoidPtr(void* data);
  ~VoidPtr() = default;

  void* get() const;
  void set(void* data);
};

} /* namespace store */

/*
enum class StoreValueType {
  STRING,
  INT,
  FLOAT,
  VOID_PTR
};

union StoreValueUnion {
  std::string s;
  int i;
  float f
  void* v;

  StoreValueUnion();
  ~StoreValueUnion();
};

class StoreValue {
 private:
  StoreValueType m_type;  
  StoreValueUnion m_value;

 public:
  StoreValue() = default;
  StoreValue(const std::string& s);
  StoreValue(int i);
  StoreValue(float f);
  StoreValue(void* v);

  StoreValueType getType() const;

  std::string& asString();
  int& asInt();
  float& asFloat();
  void*& asVoidPtr();
};
*/

using Store = std::map<std::string, store::Value*>;

} /* namespace rayc */

#endif