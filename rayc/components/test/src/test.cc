#include "core/component.h"
#include "utils/log.h"

#include <SDL2/SDL.h>

class Test : public rayc::Component {
 private:
  int m_w = 100;
  int m_h = 100;
  int m_r = 0;
  int m_g = 0;
  int m_b = 0;
  bool m_asc = true;

 public:
  Test(rayc::Context* ctx) : Component(ctx, 1, "Test", ctx->getWidth(), ctx->getHeight()) {
    show();
    setActive(true);
  }

  ~Test() {}

  void onDraw() override {
    SDL_Rect rect;
    rect.x = (getContext()->getWidth() - m_w) / 2;
    rect.y = (getContext()->getHeight() - m_h) / 2;
    rect.w = m_w;
    rect.h = m_h;

    SDL_SetRenderDrawColor(getContext()->getRenderer(), m_r, m_g, m_b, 255);
    SDL_RenderFillRect(getContext()->getRenderer(), &rect);

    updateColors();
  }

 private:
  void updateColors() {
    if (m_asc) {
      if (m_r < 255) {
        m_r++;
        return;
      }
      if (m_g < 255) {
        m_g++;
        return;
      }
      if (m_b < 255) {
        m_b++;
        return;
      }
      m_asc = false;
    } else {
      if (m_r > 0) {
        m_r--;
        return;
      }
      if (m_g > 0) {
        m_g--;
        return;
      }
      if (m_b > 0) {
        m_b--;
        return;
      }
      m_asc = true;
    }
  }

};

RAYC_GENERATE_COMPONENT_FUNCTION(Test) // Generates component creation function
