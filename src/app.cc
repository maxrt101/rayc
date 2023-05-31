#include <rayc/app.h>
#include <rayc/log.h>
#include <rayc/version.h>
#include <rayc/exception.h>
#include <rayc/math/rect.h>
#include <rayc/data/font.h>
#include <rayc/data/resources.h>
#include <rayc/renderer/font.h>
#include <rayc/renderer/utils.h>
#include <rayc/renderer/color.h>
#include <rayc/script/command.h>
#include <rayc/script/driver.h>
#include <rayc/script/builtins.h>
#include <rayc/utils/string.h>

#include <map>
#include <vector>
#include <chrono>
#include <cstdio>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using namespace rayc;

struct FrameKeyState {
  bool pressed = false;
  bool released = false;
};

static struct ApplicationState {
  bool isRunning = false;
  bool isInitialized = false;

  int screenWidth = 0;
  int screenHeight = 0;

  int cycles = 0;
  int fpsCap = 60;

  FrameKeyState keyState[322];
  bool heldKeys[322];
  bool isTextInputEnabled = false;
  bool isTextInputReady = false;
  std::string textInputLine;
  SDL_Scancode textInputEvent;

  std::vector<View*> views;

  struct ConsoleState {
    struct Entry {
      std::string text;
      SDL_Color color = RGB_WHITE;
    };
    
    bool active = false;
    int maxLines = 20;
    std::vector<Entry> lines;
    
    std::map<std::string, std::string> env {
      {"PROMPT", "] "}
    };

  } console;

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
} state;


static void resetKeys() {
  for (int i = 0; i < 322; i++) {
    state.keyState[i].pressed = false;
    state.keyState[i].released = false;
    state.heldKeys[i] = false;
  }
}

static void updateKeydown(SDL_Scancode sc) {
  if (!state.heldKeys[sc]) {
    state.keyState[sc].pressed = true;
    state.heldKeys[sc] = true;
  }
}

static void renderConsole() {
  Rect consoleBg = {0, 0, app::getWidth(), getFont("main")->getSize() * (state.console.maxLines + 1)};

  setDrawColor(0, 0, 0, 128);
  fillRect(consoleBg);

  int textYoffset = 0;

  if (state.console.lines.size() < state.console.maxLines) {
    for (auto& entry : state.console.lines) {
      getFont("main")->draw(entry.text, {0, textYoffset}, entry.color);
      textYoffset += getFont("main")->getSize();
    }
  } else {
    for (int i = state.console.lines.size() - state.console.maxLines; i < state.console.lines.size(); i++) {
      getFont("main")->draw(state.console.lines[i].text, {0, textYoffset}, state.console.lines[i].color);
      textYoffset += getFont("main")->getSize();
    }
  }
  std::string command = state.console.env["PROMPT"] + state.textInputLine;
  getFont("main")->draw(command, {0, textYoffset}, RGB_WHITE);
}

static void outputPrinter(const std::string& output) {
  auto rawLines = strsplit(output, "\n");
  std::vector<std::string> lines;
  const auto consoleWidth = app::getWidth() / getFont("main")->getWidth();

  for (auto& line : rawLines) {
    if (line.size() > consoleWidth) {
      for (size_t i = 0; i < line.size(); i += consoleWidth) {
        lines.push_back(line.substr(i, consoleWidth));
      }
    } else if (line.size()) {
      lines.push_back(line);
    }
  }

  for (auto& line : lines) {
    console::print(line, RGB_WHITE);
  }
}

static void processCommand(const std::string& command) {
  if (command.size()) {
    script::ExecutionContext ctx {state.console.env, outputPrinter};
    script::run(&ctx, command);
  }
}

void rayc::app::init(int width, int height) {
  printf("rayc v%s\n", getVersionString().c_str());

  state.screenWidth = width;
  state.screenHeight = height;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    sdlFatal("SDL_Init failed");
    throw SubsystemInitFailedException("SDL", "");
  }

  if (IMG_Init(IMG_INIT_JPG) < 0) {
    sdlFatal("IMG_Init failed");
    throw SubsystemInitFailedException("IMG", "");
  }

  if (TTF_Init() < 0) {
    sdlFatal("TTF_Init failed");
    throw SubsystemInitFailedException("TTF", "");
  }

  state.window = SDL_CreateWindow(
    "rayc",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    width,
    height,
    SDL_WINDOW_SHOWN
  );

  state.renderer = SDL_CreateRenderer(state.window, -1, 0);
  if (state.renderer == NULL) {
    sdlFatal("Renderer creation failed");
    throw ResourceCreationException("Renderer", "");
  }

  SDL_SetRenderDrawBlendMode(getRenderer(), SDL_BLENDMODE_BLEND);

  memset(&state.keyState, 0, 322 * sizeof(FrameKeyState));
  memset(&state.heldKeys, 0, 322);

  registerFont("main", new Font(DEFAULT_FONT, sizeof(DEFAULT_FONT), int((float(width) / height) * 15)));
  registerFont("main2", new Font(DEFAULT_FONT, sizeof(DEFAULT_FONT), int((float(width) / height) * 30)));
  registerFont("main3", new Font(DEFAULT_FONT, sizeof(DEFAULT_FONT), int((float(width) / height) * 45)));
  registerFont("console", new Font(DEFAULT_FONT, sizeof(DEFAULT_FONT), int((float(width) / height) * 15) ));

  registerCommand("quit", script::builtin_quit);
  registerCommand("execute", script::builtin_execute);
  registerCommand("set", script::builtin_set);
  registerCommand("unset", script::builtin_unset);
  registerCommand("env", script::builtin_env);
  registerCommand("test", script::builtin_test);
  registerCommand("expr", script::builtin_expr);
  registerCommand("clear", script::builtin_clear);
  registerCommand("print", script::builtin_print);
  registerCommand("read", script::builtin_read);
  registerCommand("map", script::builtin_map);
  registerCommand("data", script::builtin_data);
  registerCommand("view", script::builtin_view);
  registerCommand("send-event", script::builtin_sendEvent);
  registerCommand("alert", script::builtin_alert);
  registerCommand("tp", script::builtin_tp);
  registerCommand("spawn", script::builtin_spawn);
  registerCommand("kill", script::builtin_kill);
  registerCommand("ai", script::builtin_ai);

  info("rayc initialized succesfully");
  state.isInitialized = true;
}

void rayc::app::stop() {
  state.isRunning = false;
}

void rayc::app::shutdown() {
  if (!state.isInitialized) {
    return;
  }

  TTF_Quit();
  IMG_Quit();
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);
  SDL_Quit();

  state.isInitialized = false;

  info("rayc stopped");
}

void rayc::app::run() {
  auto time1 = std::chrono::system_clock::now();
  auto time2 = std::chrono::system_clock::now();
  auto time3 = std::chrono::system_clock::now();

  float frameTime = 0.1;        // Time spent rendering
  float actualFrameTime = 0.1;  // Time spent in this function during 1 frame

  setDrawColor(0, 0, 0, 255);
  clearBuffer();

  state.isRunning = true;

  while (state.isRunning) {
    time1 = std::chrono::system_clock::now();

    state.cycles++;
    float targetFrameTime = 1.0f/state.fpsCap;

    memset(&state.keyState, 0, 322*sizeof(FrameKeyState));

    state.isTextInputReady = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          state.isRunning = false;
          shutdown();
          return;
        }
        case SDL_KEYDOWN: {
          SDL_Scancode sc = event.key.keysym.scancode;
          if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE) {
            if (state.console.active) {
              resetKeys();
              state.console.active = false;
              setTextInput(false);
            } else {
              state.console.active = true;
              setTextInput(true);
            }
          } else {
            updateKeydown(sc);

            if (state.isTextInputEnabled) {
              state.textInputEvent = sc;
              switch (sc) {
                case SDL_SCANCODE_BACKSPACE: {
                  if (!state.textInputLine.empty()) {
                    state.textInputLine.pop_back();
                  }
                  break;
                }
                case SDL_SCANCODE_RETURN: {
                  state.isTextInputReady = true;
                  break;
                }
                default:
                  break;
              }
            }
            if (state.console.active && state.isTextInputReady) {
              console::print(state.console.env["PROMPT"] + state.textInputLine, RGB_YELLOW);
              processCommand(state.textInputLine);
            }
          }
          break;
        }
        case SDL_KEYUP: {
          state.keyState[event.key.keysym.scancode].released = true;
          state.heldKeys[event.key.keysym.scancode] = false;
          break;
        }
        case SDL_TEXTINPUT: {
          state.textInputLine += event.text.text;
          break;
        }
      }
    }

    clearBuffer();

    for (auto view : state.views) {
      if (view->isActive()) {
        view->onFrameUpdate(actualFrameTime);
      }
    }

    // FPS counter
    char buffer[5] = {0};
    snprintf(buffer, 5, "%4d", int(1.0f/actualFrameTime));
    getFont("main")->draw(
      std::string(buffer),
      {getWidth() - getFont("main")->getWidth() * 4, 0},
      RGB_WHITE
    );

    if (state.console.active) {
      renderConsole();
    }

    renderBuffer();

    time2 = std::chrono::system_clock::now();
    std::chrono::duration<float> frameDuration = time2 - time1;
    frameTime = frameDuration.count();

    // FPS cap
    if (frameTime < targetFrameTime) {
      SDL_Delay((targetFrameTime - frameTime) * 1000);
    }

    if (state.isTextInputReady) {
      state.isTextInputReady = false;
      state.textInputLine = "";
    }

    time3 = std::chrono::system_clock::now();
    std::chrono::duration<float> actualFrameDuration = time3 - time1;
    actualFrameTime = actualFrameDuration.count();
    // frameTime = actualFrameDuration.count();
  }
}

void rayc::app::registerView(View* view) {
  state.views.push_back(view);
}

void rayc::app::removeView(const std::string& name) {
  auto i = state.views.begin();
  for ( ; i != state.views.end(); i++) {
    if ((*i)->getName() == name) {
      delete *i;
      state.views.erase(i);
      break;
    }
  }
}

void rayc::app::clearViewStack() {
  for (auto& view : state.views) {
    delete view;
  }
  state.views.clear();
}

rayc::View* rayc::app::getView(const std::string& name) {
  for (auto& view : state.views) {
    if (view->getName() == name) {
      return view;
    }
  }

  return nullptr;
}

// void rayc::app::registerCommand(const std::string& command, Command* cmd) {
//   state.commands[command] = cmd;
// }

// void rayc::app::unregisterCommand(const std::string& command) {
//   state.commands.erase(command);
// }

// rayc::Command* rayc::app::getCommand(const std::string& command) {
//   return state.commands[command];
// }

// void rayc::app::clearCommands() {
//   for (auto& view : state.views) {
//     delete view;
//   }
//   state.views.clear();
// }

int rayc::app::getWidth() {
  return state.screenWidth;
}

int rayc::app::getHeight() {
  return state.screenHeight;
}

rayc::app::KeyState rayc::app::getKeyState(SDL_Scancode sc) {
  return KeyState {state.keyState[sc].pressed, state.heldKeys[sc], state.keyState[sc].released};
}

void rayc::console::print(const std::string& line, SDL_Color color) {
  state.console.lines.push_back({line, color});
}

void rayc::console::clear() {
  state.console.lines.clear();
}

void rayc::console::setMaxLines(int lines) {
  state.console.maxLines = lines;
}

int rayc::console::getMaxLines() {
  return state.console.maxLines;
}

void rayc::app::setTextInput(bool enable) {
  if (enable) {
    SDL_StartTextInput();
    state.isTextInputReady = false;
    state.textInputLine = "";
  } else {
    SDL_StopTextInput();
  }
  state.isTextInputEnabled = enable;
}

bool rayc::app::isTextInputEnabled() {
  return state.isTextInputEnabled;
}

bool rayc::app::isTextInputReady() {
  return state.isTextInputReady;
}

std::string rayc::app::getTextInput() {
  return state.textInputLine;
}

SDL_Scancode rayc::app::getTextInputEvent() {
  return state.textInputEvent;
}

// void setConsoleFont(Font* font);
// void printConsole(SDL_Color color, const std::string& line);
// void clearConsole();
// int getLineLimit();
// void setLineLimit(int limit);

void rayc::app::setFpsCap(int cap) {
  state.fpsCap = cap;
}

int rayc::app::getFpsCap() {
  return state.fpsCap;
}

int rayc::app::getCycles() {
  return state.cycles;
}

SDL_Window* rayc::app::getWindow() {
  return state.window;
}

SDL_Renderer* rayc::app::getRenderer() {
  return state.renderer;
}
