#include <rayc/data/resources.h>
#include <rayc/data/pak.h>
#include <rayc/config.h>
#include <map>

static std::map<std::string, rayc::Map*> gMaps;
static std::map<std::string, rayc::Texture*> gTextures;
static std::map<std::string, rayc::Font*> gFonts;
static std::map<std::string, rayc::Entity*> gEntities;
static std::map<std::string, rayc::Weapon*> gWeapons;

static struct PakMetadata {
  rayc::DataPak pak;
  rayc::Config pakConf;
  rayc::Config texturesConf;
} pakData;


rayc::PakLoadingException::PakLoadingException(const std::string& filename, const std::string& msg)
  : std::runtime_error("Pak Loading Exception in file '" + filename + "'': " + msg) {}


static std::string stringFromBuffer(const std::vector<uint8_t>& buffer) {
  std::string result;

  for (auto c : buffer) {
    result += c;
  }

  return result;
}


void rayc::loadPak(const std::string& file) {
  pakData.pak = DataPak::fromFile(file);

  if (!pakData.pak.contains("pak.conf")) {
    throw PakLoadingException(file, "Pak must contain a pak.conf file");
  }

  if (!pakData.pak.contains("res")) {
    throw PakLoadingException(file, "Pak must contain a res folder");
  }

  if (!pakData.pak["res"]->asFolder()->contains("textures")) {
    throw PakLoadingException(file, "Pak must contain a res/textures folder");
  }

  if (!pakData.pak.contains("conf")) {
    throw PakLoadingException(file, "Pak must contain a conf folder");
  }

  if (!pakData.pak["conf"]->asFolder()->contains("textures.conf")) {
    throw PakLoadingException(file, "Pak must contain a conf/textures.conf file");
  }

  if (!pakData.pak.contains("maps")) {
    throw PakLoadingException(file, "Pak must contain a maps folder");
  }

  pakData.pakConf = Config::fromString(stringFromBuffer(pakData.pak["pak.conf"]->asFile()->data));
  pakData.texturesConf = Config::fromString(stringFromBuffer(pakData.pak["conf"]->asFolder()->get("textures.conf")->asFile()->data));

  for (auto& [filename, node] : pakData.pak["maps"]->asFolder()->entries()) {
    registerMap(filename, new Map(Map::fromBytes(node->asFile()->data)));
  }

  for (auto& [filename, node] : pakData.pak["res"]->asFolder()->get("textures")->asFolder()->entries()) {
    auto file = node->asFile();
    if (pakData.texturesConf.exists(filename)) {
      registerTexture(pakData.texturesConf[filename], new Texture(file->data.data(), file->data.size()));
    }
  }

  if (pakData.pak["conf"]->asFolder()->contains("entity")) {
    for (auto& [filename, node] : pakData.pak["conf"]->asFolder()->get("entity")->asFolder()->entries()) {
      auto entity = Entity::fromConfig(Config::fromString(stringFromBuffer(node->asFile()->data)));
      registerEntity(entity->tag, entity);
    }
  }
}

void rayc::registerMap(const std::string& tag, Map* map) {
  gMaps[tag] = map;
}

void rayc::registerTexture(const std::string& tag, Texture* texture) {
  gTextures[tag] = texture;
}

void rayc::registerFont(const std::string& tag, Font* font) {
  gFonts[tag] = font;
}

void rayc::registerEntity(const std::string& tag, Entity* entity) {
  gEntities[tag] = entity;
}

void rayc::registerWeapon(const std::string& tag, Weapon* weapon) {
  gWeapons[tag] = weapon;
}

rayc::Map* rayc::getMap(const std::string& tag) {
  return gMaps[tag];
}

rayc::Texture* rayc::getTexture(const std::string& tag) {
  return gTextures[tag];
}

rayc::Font* rayc::getFont(const std::string& tag) {
  return gFonts[tag];
}

rayc::Entity* rayc::getEntity(const std::string& tag) {
  return gEntities[tag];
}

rayc::Weapon* rayc::getWeapon(const std::string& tag) {
  return gWeapons[tag];
}

void rayc::clearResources() {
  for (auto& [_, texture] : gTextures) {
    delete texture;
  }
  gTextures.clear();

  for (auto& [_, font] : gFonts) {
    delete font;
  }
  gFonts.clear();

  for (auto& [_, entity] : gEntities) {
    delete entity;
  }
  gEntities.clear();

  for (auto& [_, weapon] : gWeapons) {
    delete weapon;
  }
  gWeapons.clear();
}

void rayc::printPak() {
  printf("# pak.conf\n");
  for (auto& key : pakData.pakConf.keys()) {
    printf("%s=%s\n", key.c_str(), pakData.pakConf[key].c_str());
  }

  printf("# textures.conf\n");
  for (auto& key : pakData.texturesConf.keys()) {
    printf("%s=%s\n", key.c_str(), pakData.texturesConf[key].c_str());
  }

  printf("Maps: ");
  for (auto& [key, _] : gMaps) {
    printf("%s ", key.c_str());
  }
  printf("\n");

  printf("Textures: ");
  for (auto& [key, _] : gTextures) {
    printf("%s ", key.c_str());
  }
  printf("\n");

  printf("Entities: ");
  for (auto& [key, _] : gEntities) {
    printf("%s ", key.c_str());
  }
  printf("\n");
}
