#ifndef _RAYC_DATA_SERIALIZER_H_
#define _RAYC_DATA_SERIALIZER_H_ 1

#include <rayc/utils/abi.h>
#include <rayc/utils/tuple.h>
#include <rayc/version.h>

#include <arpa/inet.h>
#include <cstdio>
#include <string>
#include <vector>
#include <map>

namespace rayc {

struct Serializer {
  std::vector<uint8_t> buffer;

 public:
  template <typename... Args>
  inline void operator () (const std::tuple<Args...>& tuple) {
    Serializer serializer;

    tuple_for_each(tuple, serializer);
    
    buffer.insert(buffer.end(), serializer.buffer.begin(), serializer.buffer.end());
  }

  inline void operator () (uint8_t val) {
    buffer.push_back(val);
  }

  inline void operator () (uint16_t val) {
    abi::N32 n;
    n.u16[0] = htons(val);
    buffer.push_back(n.u8[0]);
    buffer.push_back(n.u8[1]);
  }

  inline void operator () (uint32_t val) {
    abi::N32 n;
    n.u32 = htonl(val);
    buffer.push_back(n.u8[0]);
    buffer.push_back(n.u8[1]);
    buffer.push_back(n.u8[2]);
    buffer.push_back(n.u8[3]);
  }

  inline void operator () (const std::string& val) {
    for (char c : val) {
      buffer.push_back(c);
    }
    buffer.push_back(0);
  }

  template <typename T>
  inline void operator () (const std::vector<T>& val) {
    (*this)((uint32_t) val.size());
    for (const auto& e : val) {
      (*this)(e);
    }
  }

  template <typename K, typename V>
  inline void operator () (const std::map<K, V>& val) {
    (*this)((uint32_t) val.size());
    for (const auto& p : val) {
      (*this)(p.first);
      (*this)(p.second);
    }
  }

  template <typename T>
  inline void operator () (const T& object) {
    (*this)(object.toTuple());
  }
};

template <typename T>
inline std::vector<uint8_t> serializeObject(const T& object) {
  Serializer serializer;

  auto tuple = object.toTuple();
  tuple_for_each(tuple, serializer);

  return serializer.buffer;
}

template <typename T>
inline int serialize(const std::string& filename, const T& object, uint32_t magic, uint32_t objectVersion) {
  std::vector<uint8_t> data = serializeObject(object);
  std::vector<uint8_t> header;

  abi::N32 n;

  auto push_u32 = [&n, &header] {
    for (int i = 0; i < 4; i++)
      header.push_back(n.u8[i]);
  };

  n.u32 = htonl(magic);
  push_u32();

  n.u32 = htonl(SERIALIZER_VERSION);
  push_u32();

  n.u32 = htonl(objectVersion);
  push_u32();

  FILE* file = fopen(filename.c_str(), "wb");

  if (!file) {
    return 1;
  }

  fwrite(header.data(), 1, header.size(), file);
  fwrite(data.data(), 1, data.size(), file);

  fclose(file);

  return 0;
}

} /* namespace rayc */

#endif /* _RAYC_DATA_SERIALIZER_H_ */