#ifndef _RAYC_CORE_LAYER_H
#define _RAYC_CORE_LAYER_H

#include <string>
#include <vector>

#include <SDL2/SDL.h>

namespace rayc {

class Application;

class Layer {
  friend class Application;

 public:
  Layer(Application* app, const std::string& name, int width, int height, int x, int y);
  Layer(const Layer&) = delete;
  virtual ~Layer();

  void move(int x, int y);
  // void resize(int w, int h);
  void hide();
  void show();
  void close();

  std::string getName() const;
  int getWidth() const;
  int getHeight() const;
  int getX() const;
  int getY() const;

 protected:
  Application* getApp() const;
  SDL_Renderer* getRenderer() const;
  void clearBuffer() const;

 public:
  virtual void onPush();
  virtual bool onDraw() = 0; // called to draw a scene, even if is not at the top level
  virtual void onFrameUpdate(float frame_time) = 0; // called only if on top
  virtual void onShow();
  virtual void onHide();

 protected:
  std::vector<Layer*> m_sublayers;

 private:
  Application* m_app;
  SDL_Texture* m_buffer = nullptr;
  std::string m_name;
  bool m_is_top_layer = false;
  bool m_is_hidden = true;
  bool m_will_close = false;
  int m_width = 0;
  int m_height = 0;
  int m_x = 0;
  int m_y = 0;
};

} /* namespace rayc */

#endif