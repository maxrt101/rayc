#include "rayc/texture.h"
#include "rayc/files/bmp.h"

err_t texture_from_file(texture_t * t, const file_t * f) {
  ASSERT_RET(t && f, E_NULL);

  if (bmp_probe(f)) {
    t->type = TEXTURE_BMP;
    t->data = bmp_get_image_data(f);
    return bmp_get_dimensions(f, &t->width, &t->height);
  }

  return E_FORMAT;
}

color_t texture_get_pixel_at(texture_t * t, int x, int y) {
  ASSERT_RET(t && t->data, COLOR_BLACK);

  switch (t->type) {
    case TEXTURE_BMP:
      return bmp_get_pixel_at(t->data, t->height, x, y);

    default:
      return COLOR_BLACK;
  }
}
