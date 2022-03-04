#include "core/app.h"
#include "core/layer.h"
#include "core/input.h"
#include "engine/raycaster.h"
#include "utils/log.h"
#include "utils/debug.h"
#include "utils/version.h"

#include "layers/base.h"
#include "layers/scene.h"
#include "layers/scenedda.h"
#include "layers/console.h"

#include <vector>
#include <iostream>

#define WIDTH  640
#define HEIGHT 480

/*
BaseLayer
 - MenuLayer
ScreenLayer
 - WeaponLayer? (can be integrated within HudLayer)
 - HudLayer
 - UpdateLayer
 - PauseMenuLayer
ConsoleLayer
*/

static void usage(const char* argv0) {
  std::cout << "rayc v" << RAYC_VERSION_STRING << " by maxrt101\n"
            << "Usage: " << argv0 << " [args] DATADIR\n"
            << "Args:\n"
            << "  -h,--help   Shows this message\n"
            << "  -w WIDTH    Set screen width\n"
            << "  -h HEIGHT   Set screen height\n"
            << "  -d          Enabled debug\n"
            << "\nIn-game manual:\n"
            << "Movement: WASD + left/right arrows to change directions\n"
            << "Once loaded, press '`' (backtick) to enter the console.\n"
            << "Console Commands:\n"
            << "  quit - quits game\n"
            << "  load MAPFILE - loads map relative to datadir\n"
            << "  get KEY - gets param value\n"
            << "  set KEY VALUE[s] - sets param value(s)\n";
}

int main(int argc, char ** argv) {
  std::string datadir;
  int width = WIDTH, height = HEIGHT;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-h")) {
      usage(argv[0]);
      return 0;
    } else if (!strcmp(argv[i], "--help")) {
      usage(argv[0]);
      return 0;
    } else if (!strcmp(argv[i], "-d")) {
      rayc::setDebug(true);
    } else if (!strcmp(argv[i], "-w")) {
      if (i+1 >= argc) {
        std::cout << "No WIDTH supplied to -w\n";
        return 1;
      }
      try {
        width = std::stoi(argv[++i]);
      } catch (const std::exception& e) {
        std::cout << "Bad number: '" << argv[i] << "'\n";
        return 1;
      }
    } else if (!strcmp(argv[i], "-h")) {
      if (i+1 >= argc) {
        std::cout << "No HEIGHT supplied to '-h'\n";
        return 1;
      }
      try {
        height = std::stoi(argv[++i]);
      } catch (const std::exception& e) {
        std::cout << "Bad number: '" << argv[i] << "'\n";
        return 1;
      }
    } else {
      if (datadir.empty()) {
        datadir = argv[i];
      } else {
        usage(argv[0]);
        return 1;
      }
    }
  }

  if (datadir.empty()) {
    usage(argv[0]);
    return 1;
  }
  
  rayc::Application app("rayc-dev", WIDTH, HEIGHT);
  rayc::Raycaster raycaster(datadir, "fonts/joystix.ttf");

  rayc::BaseLayer base(&app, WIDTH, HEIGHT, &raycaster);
  rayc::SceneLayer scene(&app, WIDTH, HEIGHT, &raycaster);
  //rayc::SceneDDALayer scene(&app, WIDTH, HEIGHT, &raycaster);
  rayc::ConsoleLayer console(&app, WIDTH, HEIGHT, &raycaster);

  app.pushLayer(&base);
  app.pushLayer(&scene);
  app.pushLayer(&console);

  app.setFpsCap(60);
  app.run();

  return 0;
}
