#ifndef _RAYC_DATA_SERIALIZATION_H_
#define _RAYC_DATA_SERIALIZATION_H_ 1

#include <rayc/utils/abi.h>
#include <rayc/utils/tuple.h>
#include <rayc/data/serializer.h>
#include <rayc/data/deserializer.h>


#define RAYC_GEN_TO_TUPLE(...) \
  auto toTuple() const { \
    auto [__VA_ARGS__] = *this; \
    return std::make_tuple(__VA_ARGS__); \
  }

#define RAYC_GEN_TO_REF_TUPLE(...) \
  auto toRefTuple() { \
    return std::tie(__VA_ARGS__); \
  }

#define RAYC_MAKE_SERIALIZABLE(...) \
  RAYC_GEN_TO_TUPLE(__VA_ARGS__) \
  RAYC_GEN_TO_REF_TUPLE(__VA_ARGS__)


namespace rayc {} /* namespace rayc */

#endif /* _RAYC_DATA_SERIALIZATION_H_ */