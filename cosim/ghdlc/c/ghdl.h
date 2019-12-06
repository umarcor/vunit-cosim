#include <stdlib.h>
#include <stdint.h>

typedef struct dim_t {
  int32_t left;
  int32_t right;
  int32_t dir;
  int32_t length;
} dim_t;

typedef struct bounds_t {
  dim_t dim_1;
} bounds_t;

typedef struct bounds2D_t {
  dim_t dim_1;
  dim_t dim_2;
} bounds2D_t;

typedef struct bounds3D_t {
  dim_t dim_1;
  dim_t dim_2;
  dim_t dim_3;
} bounds3D_t;

typedef struct fat_t {
  void* base;
  bounds_t* bounds;
} fat_t;

void printfat(fat_t* ptr) {
    printf("fat.base: %p\n", ptr->base);
    printf("fat.bounds: %p\n", ptr->bounds);
    printf("fat.bounds.left: %d\n", ptr->bounds->dim_1.left);
    printf("fat.bounds.right: %d\n", ptr->bounds->dim_1.right);
    printf("fat.bounds.dir: %d\n", ptr->bounds->dim_1.dir);
    printf("fat.bounds.length: %d\n", ptr->bounds->dim_1.length);
}

char* fatptrToString(fat_t* ptr) {
  assert(ptr != NULL);
  assert(ptr->bounds != NULL);
  int len = ptr->bounds->dim_1.length;
  char* str = malloc(sizeof(char) * len + 1);
  strncpy(str, ptr->base, len);
  str[len] = '\0';
  return str;
}

void fatptrToArray(fat_t* ptr, void** vec, int* len, int num) {
  assert(ptr != NULL);
  assert(ptr->bounds != NULL);
  *vec = ptr->base;

/*
  TODO: is it possible to provide bounds_t as a pointer of type dim_t and an integer?
        it would allow to replace the case statement below with a loop

  int x = 0;
  int y = num-1;
  while (y >= 0)
    len[y--] = ptr->bounds->dim[x++].length;
*/

  void* b = ptr->bounds;
  switch (num) {
    case 1:
      len[0] = ((bounds_t*)b)->dim_1.length;
      break;
    case 2:
      len[0] = ((bounds2D_t*)b)->dim_2.length;
      len[1] = ((bounds2D_t*)b)->dim_1.length;
      break;
    case 3:
      len[0] = ((bounds3D_t*)b)->dim_3.length;
      len[1] = ((bounds3D_t*)b)->dim_2.length;
      len[2] = ((bounds3D_t*)b)->dim_1.length;
      break;
  }
}

fat_t* fatptrFromString(char* str) {
  uint32_t len = strlen(str);
  fat_t* fat = malloc(sizeof(fat_t));
  fat->base = malloc(sizeof(char) * len);
  strncpy((char*)(fat->base), str, len);
  fat->bounds = malloc(sizeof(bounds_t));
  bounds_t* b = fat->bounds;
  b->dim_1.left = 1;
  b->dim_1.right = len;
  b->dim_1.dir = 0;
  b->dim_1.length = len;
  return fat;
}

fat_t* fatptrFromIntegerArray(void* vec, int* len, int num) {
  fat_t *fat = malloc(sizeof(fat_t));
  fat->base = vec;
  fat->bounds = malloc(sizeof(bounds_t));
  bounds_t* b = fat->bounds;
  b->dim_1.left = len[0]-1;
  b->dim_1.right = 0;
  b->dim_1.dir = 1;
  b->dim_1.length = len[0];
  return fat;
}

typedef struct line_t {
  dim_t bounds;
  char  base[];
} line_t;

line_t* lineFromString(char *str) {
  uint32_t length = strlen(str);
  line_t *line = malloc(sizeof(line_t) + sizeof(char) * length);
  line->bounds.left = 1;
  line->bounds.right = length;
  line->bounds.dir = 0;
  line->bounds.length = length;
  strncpy(line->base, str, length);
  return line;
}
