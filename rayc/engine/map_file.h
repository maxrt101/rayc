#ifndef _RAYC_ENGINE_MAP_FILE_H
#define _RAYC_ENGINE_MAP_FILE_H

#include <cstdint>
#include <string>
#include <vector>

#include "math/vec2.h"
#include "utils/optional.h"

#define RAYC_MAP_MAGIC        0xfafb
#define RAYC_MAP_FILE_VERSION 0x0001

#define RAYC_TILE_FLAG_DOOR   1

#define RAYC_DOOR_CLOSED      0
#define RAYC_DOOR_OPENED      1
#define RAYC_DOOR_OPENING     2
#define RAYC_DOOR_CLOSING     3

namespace rayc {

struct MapTile {
  uint8_t flags;
  uint8_t texture;
  uint8_t height;

  //uint8_t state[4]; // not saved in file

  bool isSolid() const;
  bool isDoor() const;
};

struct MapObject {
  uint16_t x;
  uint16_t y;
  uint8_t sprite;
};

class MapFile {
 public:
  MapFile();
  MapFile(int w, int h);

  void Save(const std::string& filename);

  static Optional<MapFile> load(const std::string& filename);

  std::string getName() const;
  int getWidth() const;
  int getHeight() const;
  int getStartX() const;
  int getStartY() const;

  void setStartX(int x);
  void setStartY(int y);

  std::vector<std::string>& getTextures();
  std::vector<std::string>& getSprites();
  std::vector<MapObject>& getObjects();

  std::vector<MapTile>& getMap();
  MapTile& getTile(int offset);
  MapTile& getTile(int x, int y);
  MapTile& getTile(Vec2i pos);

 private:
  uint16_t m_width;
  uint16_t m_height;
  uint16_t m_start_x;
  uint16_t m_start_y;
  // ceil & floor colors

  std::string m_name;

  std::vector<std::string> m_textures;
  std::vector<std::string> m_sprites;
  std::vector<MapObject> m_objects;

  std::vector<MapTile> m_map;
};

} /* namespace rayc */

#endif