#include <rayc/app.h>
#include <rayc/log.h>
#include <rayc/config.h>
#include <rayc/version.h>
#include <rayc/data/pak.h>
#include <rayc/data/resources.h>
#include <rayc/game/map.h>
#include <rayc/game/raycasting.h>
#include <rayc/ui/hud.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdio>

void usage() {
  printf(
    "Usage: ./rayc <OPTIONS>\n"
    "Usage: ./rayc [COMMAND] <ARGS>\n"
    "Options:\n"
    "  -help                  - Shows this message\n"
    "  -pak FILE              - Loads a PAK file at startup\n"
    "  -w WIDTH               - Sets window width\n"
    "  -h HEIGHT              - Sets window height\n"
    "Commands:\n"
    "  pak                    - PAK subsystem\n"
    "  pak dump FILE DIR      - Dumps contents of FILE info DIR\n"
    "  pak create FILE DIR    - Creates PAK file from DIR\n"
    "  pak ls FILE            - Lists contents of PAK file\n"
    "  pak hexdump FILE       - Prints contents of PAK file\n"
    "  map                    - MAP subsystem\n"
    "  map check FILE         - Checks if map file is valid\n"
    "  map gen FILE T1 T2 E   - Generates sample map with T1, T2 as textures, E as entity\n"
    // "  map [read|create|edit|migrate]\n"
  );
}

static bool argvCheckFlag(int argc, char ** argv, const std::string& flag) {
  for (int i = 0; i < argc; i++) {
    if (flag == argv[i]) {
      return true;
    }
  }
  return false;
} 

static bool argvCheckParam(int argc, char ** argv, const std::string& paramName, std::string& paramValue) {
  for (int i = 0; i < argc; i++) {
    if (paramName == argv[i]) {
      if (i + 1 < argc) {
        paramValue = argv[i+1];
        return true;
      } else {
        return false;
      }
    }
  }
  return false;
}

static bool argvCheckMultipleParam(int argc, char ** argv, const std::string& paramName, std::vector<std::string>& paramValues) {
  bool found = false;
  for (int i = 0; i < argc; i++) {
    if (paramName == argv[i]) {
      if (i + 1 < argc) {
        paramValues.push_back(argv[i+1]);
        found = true;
      }
    }
  }
  return found;
}

static int runSubcommand(int argc, char ** argv) {
  if (!strcmp(argv[1], "help")) {
    usage();
    return 0;
  } else if (!strcmp(argv[1], "pak")) {
    if (argc < 4) {
      usage();
      return 1;
    }

    std::string file = argv[3];

    if (!strcmp(argv[2], "dump")) {
      if (argc != 5) {
        usage();
        return 1;
      }

      rayc::DataPak pak = rayc::DataPak::fromFile(file);
      pak.dump(argv[4]);
    } else if (!strcmp(argv[2], "create")) {
      if (argc != 5) {
        usage();
        return 1;
      }

      rayc::DataPak pak = rayc::DataPak::fromFolder(argv[4]);
      pak.toFile(file);
    } else if (!strcmp(argv[2], "hexdump")) {
      rayc::DataPak pak = rayc::DataPak::fromFile(file);

      std::function<void(rayc::DataPak::Node*, std::string)> printNode =
        [&printNode](rayc::DataPak::Node* node, std::string prefixPath) {
          prefixPath = prefixPath.size() ? prefixPath + "/" : "";
          if (node->isFolder()) {
            rayc::DataPak::Folder* folder = node->asFolder();
            for (auto& [name, node] : folder->nodes) {
              printNode(node, prefixPath + folder->name);
            }
          } else {
            rayc::DataPak::File* file = node->asFile();
            printf("File '%s%s':\n", prefixPath.c_str(), file->name.c_str());
            rayc::abi::hexdump(file->data.data(), file->data.size());
          }
        };

      for (auto& [name, node] : *pak) {
        printNode(node, "");
      }
    } else if (!strcmp(argv[2], "ls")) {
      rayc::DataPak pak = rayc::DataPak::fromFile(file);

      std::function<void(rayc::DataPak::Node*, std::string)> printNode =
        [&printNode](rayc::DataPak::Node* node, std::string prefixPath) {
          prefixPath = prefixPath.size() ? prefixPath + "/" : "";
          if (node->isFolder()) {
            rayc::DataPak::Folder* folder = node->asFolder();
            for (auto& [name, node] : folder->nodes) {
              printNode(node, prefixPath + folder->name);
            }
          } else {
            rayc::DataPak::File* file = node->asFile();
            printf("%s%s\n", prefixPath.c_str(), file->name.c_str());
            // rayc::abi::hexdump(file->data.data(), file->data.size());
          }
        };

      for (auto& [name, node] : *pak) {
        printNode(node, "");
      }
    } else {
      usage();
      return 1;
    }
  } else if (!strcmp(argv[1], "map")) {
    if (argc < 4) {
      usage();
      return 1;
    }

    std::string file = argv[3];

    if (!strcmp(argv[2], "check")) {
      rayc::Map map = rayc::Map::fromFile(file);
      printf("OK\n");
    } else if (!strcmp(argv[2], "gen")) {
      if (argc != 7) {
        usage();
        return 1;
      }

      rayc::Map map;

      map.width = 16;
      map.height = 16;
      map.startX = 8;
      map.startY = 8;

      map.tiles = {
        {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1},
        {0, 1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 1, 1},
        {0, 2, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 1},
        {0, 1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 1, 1},
        {0, 2, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 1},
        {0, 1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 1, 1},
        {0, 2, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 1},
        {0, 1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 1, 1},
        {0, 2, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 1},
        {0, 1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 1, 1},
        {0, 2, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 1},
        {0, 1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 1, 1},
        {0, 2, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 1},
        {0, 1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 1, 1},
        {0, 2, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 1},
        {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1}, {0, 1, 1}, {0, 2, 1},
      };

      map.entities = {
        {4, 4, 1}
      };

      map.textureTranslations = { 
        {1, argv[4]},
        {2, argv[5]}
      };

      map.entityTypeTranslations = {
        {1, argv[6]}
      };

      map.toFile(file);
    } else {
      usage();
      return 1;
    }
  } else if (!strcmp(argv[1], "test")) { // TODO: remove
    uint32_t multi = RAYC_MULTICHAR4('R', 'M', 'A', 'P');
    printf("0x%x = %d\n", multi, multi);
  } else {
    usage();
    return 1;
  }

  return 0;
}

int main(int argc, char ** argv) {
  std::string width = "800", height = "600", pak;

  if (argc > 1) {
    if (rayc::isIn(std::string(argv[1]), "pak", "map", "test")) { // TODO: remove test
      return runSubcommand(argc, argv);
    } else {
      if (argvCheckFlag(argc, argv, "-help")) {
        usage();
        return 0;
      }
      argvCheckParam(argc, argv, "-pak", pak);
      argvCheckParam(argc, argv, "-w", width);
      argvCheckParam(argc, argv, "-h", height);
    }
  }

  rayc::app::init(stoi(width), stoi(height));

  rayc::app::registerView(new rayc::Renderer);
  rayc::app::registerView(new rayc::ui::HUD);

  if (!pak.empty()) {
    rayc::loadPak(pak);
  }

  rayc::app::run();

  rayc::app::clearViewStack();

  return 0;
}
