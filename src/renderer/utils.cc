#include <rayc/renderer/utils.h>
#include <rayc/app.h>

void rayc::setBuffer(Texture* texture) {
  if (texture) {
    SDL_SetRenderTarget(app::getRenderer(), texture->getSdlTexture());
  } else {
    SDL_SetRenderTarget(app::getRenderer(), NULL);
  }
}

void rayc::clearBuffer() {
  SDL_SetRenderDrawColor(app::getRenderer(), 0, 0, 0, 255);
  SDL_RenderClear(app::getRenderer());
}

void rayc::renderBuffer() {
  SDL_RenderPresent(app::getRenderer());
}

void rayc::setDrawColor(int r, int g, int b, int a) {
  SDL_SetRenderDrawColor(app::getRenderer(), r, g, b, a);
}

void rayc::fillRect(const Rect& rect) {
  auto sdlRect = rect.toSdlRect();
  SDL_RenderFillRect(app::getRenderer(), &sdlRect);
}

void rayc::copyTexture(Texture* texture, const Rect& src, const Rect& dest) {
  auto sdlSrc = src.toSdlRect();
  auto sdlDect = dest.toSdlRect();
  SDL_RenderCopy(app::getRenderer(), texture->getSdlTexture(), src.isUnit() ? NULL : &sdlSrc, dest.isUnit() ? NULL : &sdlDect);
}
