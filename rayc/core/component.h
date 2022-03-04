#ifndef _RAYC_CORE_COMPONENT_H
#define _RAYC_CORE_COMPONENT_H

#include <string>
#include <functional>
#include <SDL2/SDL.h>

#include "core/context.h"

#define RAYC_COMPONENT_CREATE_FUNCTION createComponent
#define RAYC_COMPONENT_CREATE_FUNCTION_STR "createComponent"

#define RAYC_GENERATE_COMPONENT_FUNCTION(name) \
  extern "C" rayc::Component* RAYC_COMPONENT_CREATE_FUNCTION(rayc::Context* ctx) { \
    return new name(ctx); \
  }

namespace rayc {

class Component {
 private:
  // Generic Component Properties
  Context* m_ctx;
  std::string m_name;
  bool m_isAttached = false;
  bool m_isHidden = false;
  bool m_isActive = false;
  int m_priority = 0;

  // Draw Properties
  SDL_Texture* m_buffer = nullptr;
  int m_width = 0;
  int m_height = 0;
  int m_xOffset = 0;
  int m_yOffset = 0;

 public:
  Component(Context* ctx, int priority, const std::string& name);
  Component(Context* ctx, int priority, const std::string& name, int width, int height, int x = 0, int y = 0);
  virtual ~Component();

  // Generic
  bool isAttached() const;
  void reattach(Context* ctx);
  void detach();

  bool isActive() const;
  void setActive(bool active);

  int getPriority() const;
  void setPriority(int priority);

  std::string getName() const;
  Context* getContext() const;

  void update(float frameTime);

  // Draw
  bool isHidden() const;
  void show();
  void hide();

  void draw() const;

  int getWidth() const;
  int getHeight() const;

  void move(int x, int y);
  int getX() const;
  int getY() const;

  static Component* fromFile(const std::string& filename, Context* ctx);

 protected:
  void clearBuffer() const;

 public:
  virtual void onAttach();
  virtual void onDetach();
  // virtual void onShow();
  // virtual void onHide();
  // virtual void onActivate();
  // virtual void onDeactive();
  virtual void onDraw();
  virtual void onFrameUpdate(float frameTime);

  friend class App;
};

// using ComponentFunction = std::function<Component*(Context*)>;
using ComponentFunction = Component*(*)(Context*); 

} /* namespace rayc */

#endif