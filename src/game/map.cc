#include <rayc/game/map.h>
#include <rayc/utils/abi.h>
#include <rayc/exception.h>

bool rayc::Map::Tile::isSolid() const {
  return height != 0;
}

rayc::Map::Tile& rayc::Map::getTile(int offset) {
  return tiles[offset];
}

rayc::Map::Tile& rayc::Map::getTile(int x, int y) {
  return tiles[y * width + x];
}

rayc::Map::Tile& rayc::Map::getTile(Vec2i pos) {
  return tiles[pos.y * width + pos.x];
}

std::vector<uint8_t> rayc::Map::toBytes() {
  return {};
}

rayc::Map rayc::Map::fromBytes(std::vector<uint8_t> bytes) {
  Map map;
  uint32_t magic, version;

  deserializeBytes(bytes, map, magic, version);

  if (magic != MAP_MAGIC || version != MAP_VERSION) {
    throw VersionMismatchException("Invalid map data");
  }

  return map;
}

void rayc::Map::toFile(const std::string& filename) {
  serialize(filename, *this, MAP_MAGIC, MAP_VERSION);
}

rayc::Map rayc::Map::fromFile(const std::string& filename) {
  Map map;
  uint32_t magic, version;

  deserialize(filename, map, magic, version);

  if (magic != MAP_MAGIC || version != MAP_VERSION) {
    throw VersionMismatchException("Invalid map file");
  }

  return map;
}
