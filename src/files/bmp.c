#include "rayc/files/bmp.h"

#include <stdio.h>

bool bmp_probe(const file_t * f) {
  ASSERT_RET(f && f->contents, NULL);

  return *(uint16_t*)f->contents == BMP_MAGIC;
}

uint8_t * bmp_get_image_data(const file_t * f) {
  ASSERT_RET(f && f->contents, NULL);

  const bmp_file_header_t * fh = (bmp_file_header_t *) f->contents;

  ASSERT_RET(fh->type == BMP_MAGIC, NULL);

  // TODO: only good for 32bit ARGB

#if 0
  bmp_info_header_t * ih = (bmp_info_header_t *) (f->contents + sizeof(bmp_file_header_t));

  printf(
    "BMP '%s'\n"
    "file.type        %x\n"
    "file.size        %d\n"
    "file.offset_bits %d\n"
    "info.size        %d\n"
    "info.width       %d\n"
    "info.height      %d\n"
    "info.planes      %d\n"
    "info.bit_count   %d\n"
    "info.compression %d\n"
    "info.size_image  %d\n",
    f->filename,
    fh->type,
    fh->size,
    fh->offset_bits,
    ih->size,
    ih->width,
    ih->height,
    ih->planes,
    ih->bit_count,
    ih->compression,
    ih->size_image
  );

  printf("contents=%p off=%d data=%p\n", f->contents, fh->offset_bits, &f->contents[fh->offset_bits]);
#endif

  return &f->contents[fh->offset_bits];
}

err_t bmp_get_dimensions(const file_t * f, int32_t * w, int32_t * h) {
  ASSERT_RET(f && f->contents, E_NULL);

  // bmp_file_header_t * fh = (bmp_file_header_t *) f->contents;
  const bmp_info_header_t * ih = (bmp_info_header_t *) (f->contents + sizeof(bmp_file_header_t));

  *w = ih->width;
  *h = ih->height;

  return E_OK;
}

color_t bmp_get_pixel_at(const uint8_t * data, int32_t h, int x, int y) {
  const uint32_t c = *(uint32_t*)&data[(y * h + x) * 4];
  return color_from_argb(c);
}
