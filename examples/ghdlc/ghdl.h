#include <stdlib.h>
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

void printfat(fat_t* ptr) {
    printf("fat.base: %p\n", ptr->base);
    printf("fat.bounds: %p\n", ptr->bounds);
    printf("fat.bounds.left: %d\n", ptr->bounds->left);
    printf("fat.bounds.right: %d\n", ptr->bounds->right);
    printf("fat.bounds.dir: %d\n", ptr->bounds->dir);
    printf("fat.bounds.length: %d\n", ptr->bounds->length);
}

char* fatptrToString(fat_t* ptr) {
  assert(ptr != NULL);
  assert(ptr->bounds != NULL);
  int len = ptr->bounds->length;
  char* str = malloc(sizeof(char) * len + 1);
  strncpy(str, ptr->base, len);
  str[len] = '\0';
  return str;
}

void fatptrToArray(fat_t* ptr, void** vec, int* len) {
  assert(ptr != NULL);
  assert(ptr->bounds != NULL);
  *len = ptr->bounds->length;
  *vec = ptr->base;
}

void fatptrTo2DArray(fat_t* ptr, void** vec, int* len, int* num) {
  assert(ptr != NULL);
  assert(ptr->bounds != NULL);
  *num = ptr->bounds[0].length;
  *len = ptr->bounds[1].length;
  *vec = ptr->base;
}

fat_t* fatptrFromString(char* str) {
  uint32_t len = strlen(str);
  fat_t *fat = malloc(sizeof(fat_t));
  fat->base = malloc(sizeof(char) * len);
  strncpy((char*)(fat->base), str, len);
  fat->bounds = malloc(sizeof(fat_t));
  fat->bounds->left = 1;
  fat->bounds->right = len;
  fat->bounds->dir = 0;
  fat->bounds->length = len;
  return fat;
}

//fat_t* fatptrFromArray(uint32_t* vec, uint32_t length) {
//
//}
