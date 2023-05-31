#ifndef _RAYC_DATA_RESOURCES_H_
#define _RAYC_DATA_RESOURCES_H_ 1

#include <rayc/renderer/texture.h>
#include <rayc/renderer/font.h>
#include <rayc/game/entity.h>
#include <rayc/game/weapon.h>
#include <rayc/game/map.h>

#include <exception>
#include <string>

namespace rayc {

struct PakLoadingException : public std::runtime_error {
  PakLoadingException(const std::string& filename, const std::string& msg);
};

void loadPak(const std::string& file);

void registerMap(const std::string& tag, Map* map);
void registerTexture(const std::string& tag, Texture* texture);
void registerFont(const std::string& tag, Font* font);
void registerEntity(const std::string& tag, Entity* entity);
void registerWeapon(const std::string& tag, Weapon* weapon);

Map* getMap(const std::string& tag);
Texture* getTexture(const std::string& tag);
Font* getFont(const std::string& tag);
Entity* getEntity(const std::string& tag);
Weapon* getWeapon(const std::string& tag);

void clearResources();

void printPak();

} /* namespace rayc */

#endif /* _RAYC_DATA_RESOURCES_H_ */