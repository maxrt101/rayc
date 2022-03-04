#include "core/component.h"
#include "utils/log.h"

#include <SDL2/SDL.h>

class Console : public rayc::Component {
 private:

 public:
  Console(rayc::Context* ctx) : Component(ctx, 1, "Console", ctx->getWidth(), ctx->getHeight()) {
    show();
    setActive(true);
  }

  ~Console() {}

  void onDraw() override {
    SDL_Rect rect;
    // rect.x = (getContext()->getWidth() - m_w) / 2;
    // rect.y = (getContext()->getHeight() - m_h) / 2;
    // rect.w = m_w;
    // rect.h = m_h;

    // SDL_SetRenderDrawColor(getContext()->getRenderer(), m_r, m_g, m_b, 255);
    // SDL_RenderFillRect(getContext()->getRenderer(), &rect); 
  }

};

RAYC_GENERATE_COMPONENT_FUNCTION(Console)
