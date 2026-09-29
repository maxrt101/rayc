#pragma once

#include "file.h"
#include "rayc/color.h"

void port_gfx_draw_pixel(int x, int y, color_t color);

err_t port_file_read_to_buffer(file_t * f, const char * filename);
err_t port_file_free_buffer(file_t * f);

void * port_file_open_for_writing(const char * filename);
err_t port_file_close_for_writing(void * f);
err_t port_file_write(void * f, size_t size, const uint8_t * data);
