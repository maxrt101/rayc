#ifndef _RAYC_APP_H_
#define _RAYC_APP_H_ 1

// #include <cstdlib>
#include <string>
#include <vector>
#include <functional>

#include <rayc/view.h>
#include <rayc/script/command.h>

#include <SDL2/SDL.h>

namespace rayc {
namespace app {

struct KeyState {
  bool pressed = false;
  bool held = false;
  bool released = false;
};

void init(int width, int height);
void stop();
void shutdown();
void run();

void registerView(View* view);
void removeView(const std::string& name);
void clearViewStack();
View* getView(const std::string& name);

int getWidth();
int getHeight();

KeyState getKeyState(SDL_Scancode sc);

void setTextInput(bool enable);
bool isTextInputEnabled();
bool isTextInputReady();
std::string getTextInput();
SDL_Scancode getTextInputEvent();

void setFpsCap(int cap);
int getFpsCap();

int getCycles();

SDL_Window* getWindow();
SDL_Renderer* getRenderer();

} /* namespace app */

namespace console {

void print(const std::string& line, SDL_Color color);
void clear();

void setMaxLines(int lines);
int getMaxLines();

} /* namespace console */
} /* namespace rayc */

#endif /* _RAYC_APP_H_ */