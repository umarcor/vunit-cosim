#include <stdint.h>

typedef struct bounds_t {
  uint32_t left;
  uint32_t right;
  uint32_t dir;
  uint32_t length;
} bounds_t;

typedef struct fat_t {
  void* base;
  bounds_t* bounds;
} fat_t;
