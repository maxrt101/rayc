#include "engine/map_file.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include "utils/log.h"
#include "utils/binary.h"

bool rayc::MapTile::isSolid() const {
  return texture != 0;
}

bool rayc::MapTile::isDoor() const {
  return flags & RAYC_TILE_FLAG_DOOR;
}

rayc::MapFile::MapFile() : MapFile(0, 0) {}

rayc::MapFile::MapFile(int w, int h) {
  m_width = w;
  m_height = h;
  m_start_x = 0;
  m_start_y = 0;
  for (int i = 0; i < w*h; i++) {
    m_map.push_back({0, 0, 1});
  }
}

void rayc::MapFile::Save(const std::string& filename) {
  std::ofstream file(filename, std::ios::out | std::ios::binary);

  uint16_t magic = RAYC_MAP_MAGIC;
  writeBinary(file, magic);
  writeBinary(file, m_width);
  writeBinary(file, m_height);
  writeBinary(file, m_start_x);
  writeBinary(file, m_start_y);

  for (char c : m_name) {
    writeBinary(file, c);
  }
  char c = '\0';
  writeBinary(file, c);

  uint8_t size = m_textures.size();
  writeBinary(file, size);
  for (auto &t : m_textures) {
    for (char c : t) {
      writeBinary(file, c);
    }
    char c = '\0';
    writeBinary(file, c);
  }

  size = m_sprites.size();
  writeBinary(file, size);
  for (auto &texture : m_sprites) {
    for (char c : texture) {
      writeBinary(file, c);
    }
    char c = '\0';
    writeBinary(file, c);
  }

  size = m_objects.size();
  writeBinary(file, size);
  for (auto &object : m_objects) {
    writeBinary(file, object.x);
    writeBinary(file, object.y);
    writeBinary(file, object.sprite);
  }

  for (auto &tile : m_map) {
    writeBinary(file, tile.flags);
    writeBinary(file, tile.texture);
    writeBinary(file, tile.height);
  }

  file.close();
}

rayc::Optional<rayc::MapFile> rayc::MapFile::load(const std::string& filename) {
  MapFile map;

  std::ifstream file(filename, std::ios::in | std::ios::binary);
  if (!file) {
    error("Invalid file '%s'", filename.c_str());
    return Optional<MapFile>();
  }

  uint16_t magic = 0;
  readBinary(file, &magic);
  /*if (magic != RAYC_MAP_MAGIC) {
    return Optional<MapFile>();
  }*/

  readBinary(file, &map.m_width);
  readBinary(file, &map.m_height);
  readBinary(file, &map.m_start_x);
  readBinary(file, &map.m_start_y);

  char c;
  readBinary(file, &c);
  while (c != '\0') {
    map.m_name.push_back(c);
    readBinary(file, &c);
  }

  uint8_t count = 0;
  readBinary(file, &count);
  for (int i = 0; i < count; i++) {
    std::string texture_name;
    readBinary(file, &c);
    while (c != '\0') {
      texture_name.push_back(c);
      readBinary(file, &c);
    }
    map.m_textures.push_back(texture_name);
  }
  
  readBinary(file, &count);
  for (int i = 0; i < count; i++) {
    std::string sprite_name;
    readBinary(file, &c);;
    while (c != '\0') {
      sprite_name.push_back(c);
      readBinary(file, &c);;
    }
    map.m_sprites.push_back(sprite_name);
  }

  readBinary(file, &count);
  for (int i = 0; i < count; i++) {
    MapObject obj;
    readBinary(file, &obj.x);
    readBinary(file, &obj.y);
    readBinary(file, &obj.sprite);
    map.m_objects.push_back(obj);
  }

  for (int i = 0; i < map.m_width*map.m_height; i++) {
    MapTile tile;
    readBinary(file, &tile.flags);
    readBinary(file, &tile.texture);
    readBinary(file, &tile.height);
    map.m_map.push_back(tile);
  }

  if (file.bad()) {
    error("Error reading file '%s'", filename.c_str());
    return Optional<MapFile>();
  }

  file.close();
  return Optional<MapFile>(map);
}

std::string rayc::MapFile::getName() const {
  return m_name;
}

int rayc::MapFile::getWidth() const {
  return m_width;
}

int rayc::MapFile::getHeight() const {
  return m_height;
}

int rayc::MapFile::getStartX() const {
  return m_start_x;
}

int rayc::MapFile::getStartY() const {
  return m_start_y;
}

void rayc::MapFile::setStartX(int x) {
  m_start_x = x;
}

void rayc::MapFile::setStartY(int y) {
  m_start_y = y;
}

std::vector<std::string>& rayc::MapFile::getTextures() {
  return m_textures;
}

std::vector<std::string>& rayc::MapFile::getSprites() {
  return m_sprites;
}

std::vector<rayc::MapObject>& rayc::MapFile::getObjects() {
  return m_objects;
}

std::vector<rayc::MapTile>& rayc::MapFile::getMap() {
  return m_map;
}

rayc::MapTile& rayc::MapFile::getTile(int offset) {
  return m_map[offset];
}

rayc::MapTile& rayc::MapFile::getTile(int x, int y) {
  return m_map[m_width * y + x];
}

rayc::MapTile& rayc::MapFile::getTile(rayc::Vec2i pos) {
  return m_map[m_width * pos.y + pos.x];
}
