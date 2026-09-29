#include "rayc/rayc.h"
#include "rayc/port.h"
#include "rayc/gfx.h"
#include "rayc/file.h"
#include "rayc/texture.h"
#include "rayc/files/bmp.h"
#include "rayc/files/resmap.h"
#include "rayc/res/font8x8.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "rayc/files/resmap.h"
#include "rayc/res/icons8x8.h"

#define SC(__sc) case SDL_ ## __sc: return __sc
static scancode_t sc_from_sdl_sc(SDL_Scancode sc) {
  switch (sc) {
    SC(SCANCODE_A);
    SC(SCANCODE_B);
    SC(SCANCODE_C);
    SC(SCANCODE_D);
    SC(SCANCODE_E);
    SC(SCANCODE_F);
    SC(SCANCODE_G);
    SC(SCANCODE_H);
    SC(SCANCODE_I);
    SC(SCANCODE_J);
    SC(SCANCODE_K);
    SC(SCANCODE_L);
    SC(SCANCODE_M);
    SC(SCANCODE_N);
    SC(SCANCODE_O);
    SC(SCANCODE_P);
    SC(SCANCODE_Q);
    SC(SCANCODE_R);
    SC(SCANCODE_S);
    SC(SCANCODE_T);
    SC(SCANCODE_U);
    SC(SCANCODE_V);
    SC(SCANCODE_W);
    SC(SCANCODE_X);
    SC(SCANCODE_Y);
    SC(SCANCODE_Z);
    SC(SCANCODE_1);
    SC(SCANCODE_2);
    SC(SCANCODE_3);
    SC(SCANCODE_4);
    SC(SCANCODE_5);
    SC(SCANCODE_6);
    SC(SCANCODE_7);
    SC(SCANCODE_8);
    SC(SCANCODE_9);
    SC(SCANCODE_0);
    SC(SCANCODE_RETURN);
    SC(SCANCODE_ESCAPE);
    SC(SCANCODE_BACKSPACE);
    SC(SCANCODE_TAB);
    SC(SCANCODE_SPACE);
    SC(SCANCODE_MINUS);
    SC(SCANCODE_EQUALS);
    SC(SCANCODE_LEFTBRACKET);
    SC(SCANCODE_RIGHTBRACKET);
    SC(SCANCODE_SEMICOLON);
    SC(SCANCODE_APOSTROPHE);
    SC(SCANCODE_GRAVE);
    SC(SCANCODE_COMMA);
    SC(SCANCODE_PERIOD);
    SC(SCANCODE_SLASH);
    SC(SCANCODE_F1);
    SC(SCANCODE_F2);
    SC(SCANCODE_F3);
    SC(SCANCODE_F4);
    SC(SCANCODE_F5);
    SC(SCANCODE_F6);
    SC(SCANCODE_F7);
    SC(SCANCODE_F8);
    SC(SCANCODE_F9);
    SC(SCANCODE_F10);
    SC(SCANCODE_F11);
    SC(SCANCODE_F12);
    SC(SCANCODE_RIGHT);
    SC(SCANCODE_LEFT);
    SC(SCANCODE_DOWN);
    SC(SCANCODE_UP);
    SC(SCANCODE_LCTRL);
    SC(SCANCODE_LSHIFT);
    SC(SCANCODE_LALT);
    SC(SCANCODE_RCTRL);
    SC(SCANCODE_RSHIFT);
    SC(SCANCODE_RALT);
    case SDL_SCANCODE_LGUI: return SCANCODE_LMETA;
    case SDL_SCANCODE_RGUI: return SCANCODE_RMETA;
    default:
      return SCANCODE_UNKNOWN;
  }
}
#undef SC

static struct {
  SDL_Window *   window;
  SDL_Renderer * renderer;
} sdl;

static void sdl_init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    die("SDL_Init failed: %s", SDL_GetError());
  }

#if 0
  if (IMG_Init(IMG_INIT_JPG) < 0) {
    die("IMG_Init failed: %s", SDL_GetError());
  }

  if (TTF_Init() < 0) {
    die("TTF_Init failed: %s", SDL_GetError());
  }
#endif

  sdl.window = SDL_CreateWindow(
    "rayc",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WIDTH * SCALE,
    HEIGHT * SCALE,
    SDL_WINDOW_SHOWN
  );

  if (!sdl.window) {
    die("SDL_CreateWindow failed: %s", SDL_GetError());
  }

  sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);

  if (!sdl.renderer) {
    die("SDL_CreateRenderer failed: %s", SDL_GetError());
  }

  SDL_SetRenderDrawBlendMode(sdl.renderer, SDL_BLENDMODE_BLEND);
}

static void sdl_deinit() {
#if 0
  TTF_Quit();
  IMG_Quit();
#endif
  SDL_DestroyRenderer(sdl.renderer);
  SDL_DestroyWindow(sdl.window);
  SDL_Quit();
}

void port_gfx_draw_pixel(const int x, const int y, const color_t color) {
  SDL_SetRenderDrawColor(sdl.renderer, color.r, color.g, color.b, color.a);

  SDL_Rect p = {
    .x = x * SCALE,
    .y = y * SCALE,
    .w = SCALE,
    .h = SCALE
  };

  SDL_RenderFillRect(sdl.renderer, &p);
}

err_t port_file_read_to_buffer(file_t * f, const char * filename) {
  memset(f, 0, sizeof(*f));
  strncpy(f->filename, filename, MAX_FILENAME);

  FILE * fp = fopen(filename, "rb");
  if (!fp) {
    return E_IO;
  }

  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  rewind(fp);

  f->size = size;
  f->contents = malloc(size);

  if (fread(f->contents, size, 1, fp) != 1) {
    free(f->contents);
    memset(f, 0, sizeof(*f));
    return E_IO;
  }

  fclose(fp);

  return E_OK;
}

err_t port_file_free_buffer(file_t * f) {
  if (!f || !f->contents) return E_NULL;

  free(f->contents);
  memset(f, 0, sizeof(*f));

  return E_OK;
}

void * port_file_open_for_writing(const char * filename) {
  return fopen(filename, "wb");
}

err_t port_file_close_for_writing(void * f) {
  return fclose(f) == 0 ? E_OK : E_IO;
}

err_t port_file_write(void * f, size_t size, const uint8_t * data) {
  return fwrite(data, size, 1, f) == 1 ? E_OK : E_IO;
}

static void gen_test_map(const char * path) {
  sector_t sectors[] = {
    { .ws = 0,  .we = 4,  .z1 = 0, .z2 = 40, .tf = 0, .tc = 1 },
    { .ws = 4,  .we = 8,  .z1 = 0, .z2 = 40, .tf = 3, .tc = 2 },
    { .ws = 8,  .we = 12, .z1 = 0, .z2 = 40, .tf = 4, .tc = 5 },
    { .ws = 12, .we = 16, .z1 = 0, .z2 = 40, .tf = 4, .tc = 5 },
  };

  wall_t walls[] = {
    // Sector 0
    {  0,  0, 32,  0, 0, 1, 1 },
    { 32,  0, 32, 32, 1, 2, 1 },
    { 32, 32,  0, 32, 0, 1, 2 },
    {  0, 32,  0,  0, 1, 2, 2 },

    // Sector 1
    { 64,  0, 96,  0, 0, 1, 1 },
    { 96,  0, 96, 32, 1, 1, 1 },
    { 96, 32, 64, 32, 0, 1, 1 },
    { 64, 32, 64,  0, 1, 1, 1 },

    // Sector 2
    { 64, 64, 96, 64, 2, 1, 1 },
    { 96, 64, 96, 96, 3, 1, 1 },
    { 96, 96, 64, 96, 2, 1, 1 },
    { 64, 96, 64, 64, 3, 1, 1 },

    // Sector 3
    {  0, 64, 32, 64, 0, 1, 1 },
    { 32, 64, 32, 128, 0, 2, 1 },
    { 32, 128,  0, 128, 0, 1, 1 },
    {  0, 128,  0, 64, 0, 2, 1 },
  };

  map_t map = {
    .start = { .x = 70, .y = -110, .z = 20 },
    .sector_count = ARR_COUNT(sectors),
    .wall_count = ARR_COUNT(walls),
    .sectors = sectors,
    .walls = walls,
  };

  void * mf = file_open_for_writing(path);
  map_to_file(&map, mf);
  file_close_for_writing(mf);

  printf("Written test map to %s\n", path);
}

static void gen_test_resmap(const char * path) {
  const char * textures[] = {
    "res/wall1.bmp",
    "res/wall2.bmp",
    "res/fireblue1.bmp",
    "res/fireblue2.bmp",
    "res/metal1.bmp",
    "res/metal2.bmp"
  };

  void * f = file_open_for_writing(path);

  file_write_u4(f, RESMAP_MAGIC);
  file_write_u2(f, ARR_COUNT(textures));

  for (int i = 0; i < ARR_COUNT(textures); ++i) {
    file_write(f, strlen(textures[i]) + 1, (const uint8_t *)textures[i]);
  }

  file_close_for_writing(f);

  printf("Written test resmap to %s\n", path);
}

#define ARG_CHK() ({ if (i + 1 >= argc) { fprintf(stderr, "%s expect an argument\n", argv[i]); return 1; } })

int main(int argc, char ** argv) {
  const char * init_map = NULL;
  const char * init_resmap = NULL;

  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "-help")) {
      printf("Usage: rayc [-help] [-map FILE] [-resmap FILE] [-gen-test-map FILE] [-gen-test-resmap FILE]\n");
      return 0;
    }

    if (!strcmp(argv[i], "-gen-test-map")) {
      ARG_CHK();
      gen_test_map(argv[++i]);
      return 0;
    }

    if (!strcmp(argv[i], "-gen-test-resmap")) {
      ARG_CHK();
      gen_test_resmap(argv[++i]);
      return 0;
    }

    if (!strcmp(argv[i], "-map")) {
      ARG_CHK();
      init_map = argv[++i];
    }

    if (!strcmp(argv[i], "-resmap")) {
      ARG_CHK();
      init_resmap = argv[++i];
    }
  }

  printf("rayc\n");

  sdl_init();

  rayc_t rayc;

  rayc_init(&rayc);

  if (init_resmap) {
    file_t resmap;
    if (file_read_to_buffer(&resmap, init_resmap)) { die("Failed to open resmap %s", init_resmap); }
    resmap_load(&rayc, &resmap);
  }

  if (init_map) {
    rayc.map_file = rayc_file_alloc(&rayc);
    if (file_read_to_buffer(rayc.map_file, init_map)) { die("Failed to open map %s", init_map); }
    if (map_from_file(&rayc.map, rayc.map_file)) { die("Failed to load map %s", init_map); }

    rayc.player.pos.x = rayc.map.start.x;
    rayc.player.pos.y = rayc.map.start.y;
    rayc.player.pos.z = rayc.map.start.z;
  }

  uint32_t ticks = 0;
  uint16_t fps = 0;

  while (rayc.running) {
    ticks++;

    const uint64_t frame_start = SDL_GetPerformanceCounter();

    input_reset_press_release(&rayc.input);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          rayc.running = false;
          break;
        }

        case SDL_KEYDOWN: {
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            rayc.running = false;
          }

          const scancode_t sc = sc_from_sdl_sc(event.key.keysym.scancode);

          if (!sc) break;

          // printf("DOWN %2d '%c'\n", sc, scancode2char(sc));

          input_key_down(&rayc.input, sc);

          break;
        }

        case SDL_KEYUP: {
          const scancode_t sc = sc_from_sdl_sc(event.key.keysym.scancode);

          if (!sc) break;

          // printf("UP   %2d '%c'\n", sc, scancode2char(sc));

          input_key_up(&rayc.input, sc);

          break;
        }

        case SDL_MOUSEMOTION: {
          rayc.input.mouse.x = event.motion.x / SCALE;
          rayc.input.mouse.y = event.motion.y / SCALE;
          break;
        }

        case SDL_MOUSEBUTTONDOWN: {
          if (event.button.button == SDL_BUTTON_LEFT)  { rayc.input.mouse.left_btn = true;  }
          if (event.button.button == SDL_BUTTON_RIGHT) { rayc.input.mouse.right_btn = true; }
          break;
        }

        case SDL_MOUSEBUTTONUP: {
          if (event.button.button == SDL_BUTTON_LEFT)  { rayc.input.mouse.left_btn = false;  }
          if (event.button.button == SDL_BUTTON_RIGHT) { rayc.input.mouse.right_btn = false; }
          break;
        }

        default:
          break;
      }
    }

    // SDL_SetRenderDrawColor(sdl.renderer, 130, 130, 130, 255);
    SDL_SetRenderDrawColor(sdl.renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl.renderer);

    rayc_main(&rayc);

    // Show Debug FPS
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", fps);

    gfx_draw_string(&font8x8, WIDTH - font_calc_str_width(&font8x8, buf), 0, COLOR_WHITE, buf);

    // Draw debug cursor
    color_t cursor = RGB(255, 255, 255);
#if WITH_EDITOR
    if (rayc.editor.mode == EDITOR_MODE_DRAW)   cursor = RGB(0, 150, 0);
    if (rayc.editor.mode == EDITOR_MODE_DELETE) cursor = RGB(150, 0, 0);
#endif
    gfx_draw_char(&icons8x8, rayc.input.mouse.x - 4, rayc.input.mouse.y - 4, cursor, ICONS8X8_CURSOR);

    // Render frame
    SDL_RenderPresent(sdl.renderer);

    // Calculate frame time -> fps & perform fps lock on 30
    uint64_t frame_end = SDL_GetPerformanceCounter();

    float elapsed_seconds = (float)(frame_end - frame_start) / SDL_GetPerformanceFrequency();
    const uint32_t frame_time_ms = (uint32_t)(elapsed_seconds * 1000.0f);

    if (frame_time_ms < TARGET_FRAME_TIME_MS) {
      SDL_Delay(TARGET_FRAME_TIME_MS - frame_time_ms);

      frame_end = SDL_GetPerformanceCounter();
      elapsed_seconds = (float)(frame_end - frame_start) / SDL_GetPerformanceFrequency();
    }

    fps = elapsed_seconds > 0.0f ? (int)(1.0f / elapsed_seconds) : 0;
  }

  rayc_deinit(&rayc);
  sdl_deinit();
  return 0;
}
