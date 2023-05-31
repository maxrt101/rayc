#include <rayc/utils/abi.h>

#include <cstring>
#include <cstdio>
#include <cctype>

void rayc::abi::readInt16(N32& result, const uint8_t* data, size_t offset) {
  result.u8[0] = data[offset];
  result.u8[1] = data[offset+1];
}

void rayc::abi::readInt32(N32& result, const uint8_t* data, size_t offset) {
  result.u8[0] = data[offset];
  result.u8[1] = data[offset+1];
  result.u8[2] = data[offset+2];
  result.u8[3] = data[offset+3];
}

void rayc::abi::hexdump(const unsigned char* data, size_t size, int cols) {
  int can_print = 1;
  size_t count = 0;

  char* buf = new char[cols];
  while (can_print) {
    memset(buf, '.', cols);

    printf("0x%04zx |", count);

    for (int i = 0; i < cols; i++) {
      printf("%s", (i % (cols/2) == 0 && i != 0) ? "  " : " ");

      if (count < size) {
        printf("%02x", (unsigned char)data[count]);
        buf[count % cols] = data[count];
        count++;
      } else {
        printf("  ");
        can_print = 0;
      }
    }

    printf(" | ");

    for (int i = 0; i < cols; i++) {
      printf("%c", isprint(buf[i]) ? buf[i] : '.');
    }

    printf("\n");
  }
  delete [] buf;
}
