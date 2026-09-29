#pragma once

#include "rayc/file.h"
#include "rayc/err.h"

#define RESMAP_MAGIC 0x52524553 // b"RRES"

#define RESMAP_MAX_NAME 32

typedef struct rayc_s rayc_t;

err_t resmap_load(rayc_t * rayc, file_t * f);
