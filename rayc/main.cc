#include "core/app.h"
#include "utils/shared_library.h"
#include "utils/config.h"
#include "utils/log.h"

#include <iostream>

int main(int argc, char** argv) {
  try {
    if (argc != 2) {
      rayc::error("Usage: %s CONFIG_FILE", argv[0]);
      return 1;
    }

    auto config = rayc::Config::fromFile(argv[1]);

    /*if (config.exists("core", "logLevel")) {
      rayc::logger::stringToLogLevel(config.getValue("core", "logLevel"));
    }*/

    config.ifExists("core", "logLevel", [](auto value) {
      rayc::logger::stringToLogLevel(value);
    });

    std::string mainComponentName = config.getValueOr("core", "component", "");
    if (mainComponentName.empty()) {
      rayc::fatal("Please provide core.component property");
      return 1;
    }

    auto mainComponent = rayc::SharedLibrary::load(mainComponentName);

    rayc::Application app(config);
    app.attachComponent(rayc::Component::fromFile(mainComponentName, app.createContext()));
    app.run();

  } catch (std::exception& e) {
    rayc::fatal("Uncaught exception: %s", e.what());
    return 1;
  }
}
