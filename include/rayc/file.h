#pragma once

#include "rayc/defs.h"
#include "rayc/err.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  char      filename[MAX_FILENAME];
  size_t    size;
  size_t    ofs;
  uint8_t * contents;
} file_t;

err_t file_read_to_buffer(file_t * f, const char * filename);
err_t file_free_buffer(file_t * f);

void file_rewind(file_t * f);

int8_t   file_read_i1(file_t * f);
uint8_t  file_read_u1(file_t * f);
int16_t  file_read_i2(file_t * f);
uint16_t file_read_u2(file_t * f);
int32_t  file_read_i4(file_t * f);
uint32_t file_read_u4(file_t * f);

void * file_open_for_writing(const char * filename);
err_t file_close_for_writing(void * f);
err_t file_write(void * f, size_t size, const uint8_t * data);

err_t file_write_i1(void * f, int8_t   value);
err_t file_write_u1(void * f, uint8_t  value);
err_t file_write_i2(void * f, int16_t  value);
err_t file_write_u2(void * f, uint16_t value);
err_t file_write_i4(void * f, int32_t  value);
err_t file_write_u4(void * f, uint32_t value);
