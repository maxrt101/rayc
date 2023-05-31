#ifndef _RAYC_RENDERER_UTILS_H_
#define _RAYC_RENDERER_UTILS_H_ 1

#include <rayc/math/rect.h>
#include <rayc/renderer/texture.h>

namespace rayc {

void setBuffer(Texture* texture);
void clearBuffer();
void renderBuffer();
void setDrawColor(int r, int g, int b, int a = 255);
void fillRect(const Rect& rect);
void copyTexture(Texture* texture, const Rect& src, const Rect& dest);

} /* namespace rayc */

#endif /* _RAYC_RENDERER_UTILS_H_ */