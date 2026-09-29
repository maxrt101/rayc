#include "rayc/files/resmap.h"
#include "rayc/rayc.h"
#include "rayc/util.h"
#include <stdint.h>

err_t resmap_load(rayc_t * rayc, file_t * f) {
  ASSERT_RET(rayc && f, E_NULL);

  if (file_read_u4(f) != RESMAP_MAGIC) {
    return E_FORMAT;
  }

  uint16_t texture_count = file_read_u2(f);

  for (int i = 0; i < texture_count; ++i) {
    char path[RESMAP_MAX_NAME];
    int idx = 0;

    uint8_t c = 0;
    do {
      c = file_read_u1(f);
      path[idx++] = c;
    } while (c);

    file_t * texture_file = rayc_file_alloc(rayc);

    ASSERT(texture_file, "Out Of File Handles (%d)", MAX_FILES);

    if (file_read_to_buffer(texture_file, path)) { die("Failed to open %s", path); }
    if (texture_from_file(&rayc->textures[i], texture_file)) { die("Failed to process texture %s", path); }
  }

  return E_OK;
}
