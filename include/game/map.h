#ifndef _RAYC_GAME_MAP_H_
#define _RAYC_GAME_MAP_H_ 1

#include <rayc/data/serialization.h>
#include <rayc/math/vec2.h>

#include <cstdint>
#include <string>
#include <vector>
#include <map>

namespace rayc {

constexpr uint32_t MAP_MAGIC = RAYC_MULTICHAR4('R', 'M', 'A', 'P');
constexpr uint32_t MAP_VERSION = 0x00000001;

class Map {
 public:
  struct Tile {
    uint8_t flags;
    uint8_t texture;
    uint8_t height;

    bool isSolid() const;
    // bool isDoor() const;

    RAYC_MAKE_SERIALIZABLE(flags, texture, height);
  };

  struct Entity {
    uint16_t x, y;
    uint8_t type;

    RAYC_MAKE_SERIALIZABLE(x, y, type);
  };

 public:
  uint16_t width;
  uint16_t height;
  uint16_t startX;
  uint16_t startY;

  std::vector<Tile> tiles;
  std::vector<Entity> entities;

  std::map<uint8_t, std::string> textureTranslations;
  std::map<uint8_t, std::string> spriteTranslations;
  std::map<uint8_t, std::string> entityTypeTranslations;

  RAYC_MAKE_SERIALIZABLE(
    width, height, startX, startY,
    tiles, entities,
    textureTranslations, spriteTranslations, entityTypeTranslations
  );

 public:
  Tile& getTile(int offset);
  Tile& getTile(int x, int y);
  Tile& getTile(Vec2i pos);

  std::vector<uint8_t> toBytes();
  static Map fromBytes(std::vector<uint8_t> bytes);

  void toFile(const std::string& filename);
  static Map fromFile(const std::string& filename);
};

} /* namespace rayc */

#endif /* _RAYC_GAME_MAP_H_ */