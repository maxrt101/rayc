#include "rayc/file.h"
#include "rayc/port.h"

err_t file_read_to_buffer(file_t * f, const char * filename) {
  return port_file_read_to_buffer(f, filename);
}

err_t file_free_buffer(file_t * f) {
  return port_file_free_buffer(f);
}

void file_rewind(file_t * f) {
  ASSERT_RET(f);

  f->ofs = 0;
}

int8_t file_read_i1(file_t * f) {
  ASSERT_RET(f, 0);

  const int8_t res = (int8_t)f->contents[f->ofs];
  f->ofs += 1;
  return res;
}

uint8_t file_read_u1(file_t * f) {
  ASSERT_RET(f, 0);

  const uint8_t res = f->contents[f->ofs];
  f->ofs += 1;
  return res;
}

int16_t file_read_i2(file_t * f) {
  ASSERT_RET(f, 0);

  const int16_t res =
    (int16_t)f->contents[f->ofs] << 8 |
             f->contents[f->ofs+1];
  f->ofs += 2;
  return res;
}

uint16_t file_read_u2(file_t * f) {
  ASSERT_RET(f, 0);

  const uint16_t res =
    (uint16_t)f->contents[f->ofs] << 8 |
              f->contents[f->ofs+1];
  f->ofs += 2;
  return res;
}

int32_t file_read_i4(file_t * f) {
  ASSERT_RET(f, 0);

  const int32_t res =
    (int32_t)f->contents[f->ofs]   << 24 |
    (int32_t)f->contents[f->ofs+1] << 16 |
    (int32_t)f->contents[f->ofs+2] << 8 |
             f->contents[f->ofs+3];
  f->ofs += 4;
  return res;
}

uint32_t file_read_u4(file_t * f) {
  ASSERT_RET(f, 0);
  const uint32_t res =
    (uint32_t)f->contents[f->ofs]   << 24 |
    (uint32_t)f->contents[f->ofs+1] << 16 |
    (uint32_t)f->contents[f->ofs+2] << 8 |
              f->contents[f->ofs+3];
  f->ofs += 4;
  return res;
}

void * file_open_for_writing(const char * filename) {
  return port_file_open_for_writing(filename);
}

err_t file_close_for_writing(void * f) {
  return port_file_close_for_writing(f);
}

err_t file_write(void * f, const size_t size, const uint8_t * data) {
  return port_file_write(f, size, data);
}


err_t file_write_i1(void * f, const int8_t value) {
  return file_write(f, 1, (uint8_t*)&value);
}

err_t file_write_u1(void * f, const uint8_t value) {
  return file_write(f, 1, &value);
}

err_t file_write_i2(void * f, const int16_t value) {
  const uint8_t buf[2] = {
    (value & 0xFF00) >> 8,
    value & 0xFF
  };

  return file_write(f, 2, buf);
}

err_t file_write_u2(void * f, const uint16_t value) {
  const uint8_t buf[2] = {
    (value & 0xFF00) >> 8,
    value & 0xFF
  };

  return file_write(f, 2, buf);
}

err_t file_write_i4(void * f, const int32_t value) {
  const uint8_t buf[4] = {
    (value & 0xFF000000) >> 24,
    (value & 0x00FF0000) >> 16,
    (value & 0x0000FF00) >> 8,
    value & 0xFF
  };

  return file_write(f, 4, buf);
}

err_t file_write_u4(void * f, const uint32_t value) {
  const uint8_t buf[4] = {
    (value & 0xFF000000) >> 24,
    (value & 0x00FF0000) >> 16,
    (value & 0x0000FF00) >> 8,
    value & 0xFF
  };

  return file_write(f, 4, buf);
}


