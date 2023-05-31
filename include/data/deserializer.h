#ifndef _RAYC_DATA_DESERIALIZER_H_
#define _RAYC_DATA_DESERIALIZER_H_ 1

#include <rayc/utils/abi.h>
#include <rayc/utils/tuple.h>
#include <rayc/version.h>

#include <arpa/inet.h>
#include <cstdio>
#include <string>
#include <vector>
#include <map>

namespace rayc {

struct Deserializer {
  std::vector<uint8_t>& buffer;
  size_t offset = 0;

 public:
  inline Deserializer(std::vector<uint8_t>& buffer, size_t offset = 0) : buffer(buffer), offset(offset) {}
 
  inline void operator () (uint8_t& val) {
    val = buffer[offset];
    offset++;
  }

  inline void operator () (uint16_t& val) {
    abi::N32 n;
    n.u8[0] = buffer[offset++];
    n.u8[1] = buffer[offset++];
    val = ntohs(n.u16[0]);
  }

  inline void operator () (uint32_t& val) {
    abi::N32 n;
    n.u8[0] = buffer[offset++];
    n.u8[1] = buffer[offset++];
    n.u8[2] = buffer[offset++];
    n.u8[3] = buffer[offset++];
    val = ntohl(n.u32);
  }

  inline void operator () (std::string& val) {
    while (buffer[offset] != 0) {
      val.push_back(buffer[offset++]);
    }
    offset++;
  }

  template <typename T>
  inline void operator () (std::vector<T>& val) {
    uint32_t size = 0;
    (*this)(size);

    for (uint32_t i = 0; i < size; i++) {
      T e;
      (*this)(e);
      val.push_back(e);
    }
  }

  template <typename K, typename V>
  inline void operator () (std::map<K, V>& val) {
    uint32_t size = 0;
    (*this)(size);

    for (uint32_t i = 0; i < size; i++) {
      K k; V v;
      (*this)(k);
      (*this)(v);
      val[k] = v;
    }
  }

  template <typename T>
  inline void operator () (T& object) {
    auto tuple = object.toRefTuple();

    Deserializer deserializer(buffer, offset);
    
    tuple_for_each(tuple, deserializer);

    object = tuple_to_struct<T>(tuple);

    offset = deserializer.offset;
  }
};

template <typename T>
inline void deserializeObject(std::vector<uint8_t> buffer, T& object) {
  Deserializer deserializer(buffer);

  auto tuple = object.toRefTuple();

  tuple_for_each(tuple, deserializer);

  object = tuple_to_struct<T>(tuple);
}

template <typename T>
inline int deserializeBytes(const std::vector<uint8_t>& bytes, T& object, uint32_t& magic, uint32_t& objectVersion) {
  abi::N32 n;
  size_t offset = 0;

  auto load_u32 = [&n, &bytes, &offset] {
    for (int i = 0; i < 4; i++)
      n.u8[i] = bytes[offset++];
  };

  load_u32();
  magic = ntohl(n.u32);

  load_u32();
  uint32_t serializerVersion = ntohl(n.u32);

  load_u32();
  objectVersion = ntohl(n.u32);

  if (serializerVersion != SERIALIZER_VERSION) {
    return 3;
  }

  std::vector<uint8_t> objectData = std::vector(bytes.begin() + sizeof(uint32_t) * 3, bytes.end());

  deserializeObject(objectData, object);

  return 0;
}

template <typename T>
inline int deserialize(const std::string& filename, T& object, uint32_t& magic, uint32_t& objectVersion) {
  FILE* file = fopen(filename.c_str(), "rb");
  if (!file) {
    return 1;
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  uint8_t* data = new uint8_t[size];
  if (fread(data, 1, size, file) != size) {
    delete [] data;
    fclose(file);
    return 2;
  }

  std::vector<uint8_t> buffer(data, data + size);

  int ret = deserializeBytes(buffer, object, magic, objectVersion);

  delete [] data;
  fclose(file);

  return ret;
}

} /* namespace rayc */

#endif /* _RAYC_DATA_DESERIALIZER_H_ */