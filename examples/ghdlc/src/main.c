#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include <ghdl.h>

typedef struct rec_t {
  char    r_char;
  int32_t r_int;
} rec_t;

typedef enum {standby, start, busy, done} enum_t;

void testCinterface(
  char     v_char,
  int32_t  v_int,
  uint32_t v_nat,
  uint32_t v_pos,
  double   v_real,
  bool     v_bool,
  bool     v_bit,
  int64_t  v_time,
  rec_t*   v_rec,
  uint8_t  v_enum,
  fat_t*   v_str,
  fat_t*   v_vec_int,
  fat_t*   v_vec_real,
  fat_t*   v_vec_bool,
  fat_t*   v_vec_bit,
  fat_t*   v_vec_phy,
  fat_t*   v_vec_rec,
  fat_t*   v_vec_enum,
  fat_t*   v_2vec_real
) {
  assert(v_char == 'k');
  printf("v_char : %c\n", v_char);

  assert(v_int == -6);
  printf("v_int  : %d\n", v_int);

  assert(v_nat == 9);
  printf("v_nat  : %d\n", v_nat);

  assert(v_pos == 3);
  printf("v_pos  : %d\n", v_pos);

  assert(v_real == 3.34);
  printf("v_real : %f\n", v_real);

  assert(v_bool == true);
  printf("v_bool : %d\n", v_bool);

  assert(v_bit == true);
  printf("v_bit  : %d\n", v_bit);

  assert(v_time == 20e6);
  printf("v_time : %d\n", v_time);

  assert(v_rec != NULL);
  assert(v_rec->r_char == 'y');
  assert(v_rec->r_int == 5);
  printf("v_rec  : %p %c %d\n", v_rec, v_rec->r_char, v_rec->r_int);

  assert(v_enum == busy);
  printf("v_enum : %d %d\n", v_enum, busy);

  char* str = fatptrToString(v_str);
  printf("v_str  : %p '%s' [%d]\n", v_str->base, str, strlen(str));

  int* len = malloc(2 * sizeof(int));

  int32_t* vec_int;
  fatptrToArray(v_vec_int, (void**)&vec_int, len, 1);
  assert(vec_int[0] == 11);
  assert(vec_int[1] == 22);
  assert(vec_int[2] == 33);
  assert(vec_int[3] == 44);
  assert(vec_int[4] == 55);
  printf("v_vec_int  : %p [%d]\n", vec_int, len[0]);

  double* vec_real;
  fatptrToArray(v_vec_real, (void**)&vec_real, len, 1);
  assert(vec_real[0] == 0.5);
  assert(vec_real[1] == 1.75);
  assert(vec_real[2] == 3.33);
  assert(vec_real[3] == -0.125);
  assert(vec_real[4] == -0.67);
  assert(vec_real[5] == -2.21);
  printf("v_vec_real : %p [%d]\n", vec_real, len[0]);

  bool* vec_bool;
  fatptrToArray(v_vec_bool, (void**)&vec_bool, len, 1);
  assert(vec_bool[0] == 0);
  assert(vec_bool[1] == 1);
  assert(vec_bool[2] == 1);
  assert(vec_bool[3] == 0);
  printf("v_vec_bool : %p [%d]\n", vec_bool, len[0]);

  bool* vec_bit;
  fatptrToArray(v_vec_bit, (void**)&vec_bit, len, 1);
  assert(vec_bit[0] == 1);
  assert(vec_bit[1] == 0);
  assert(vec_bit[2] == 1);
  assert(vec_bit[3] == 0);
  printf("v_vec_bit  : %p [%d]\n", vec_bit, len[0]);

  int64_t* vec_phy;
  fatptrToArray(v_vec_phy, (void**)&vec_phy, len, 1);
  assert(vec_phy[0] == 1e6);
  assert(vec_phy[1] == 50e3);
  assert(vec_phy[2] == 1.34e9);
  printf("v_vec_phy  : %p [%d]\n", vec_phy, len[0]);

  rec_t* vec_rec;
  fatptrToArray(v_vec_rec, (void**)&vec_rec, len, 1);
  assert(vec_rec[0].r_char == 'x');
  assert(vec_rec[0].r_int == 17);
  assert(vec_rec[1].r_char == 'y');
  assert(vec_rec[1].r_int == 25);
  printf("v_vec_rec  : %p [%d]\n", vec_rec, len[0]);

  uint8_t* vec_enum;
  fatptrToArray(v_vec_enum, (void**)&vec_enum, len, 1);
  assert(vec_enum[0] == start);
  assert(vec_enum[1] == busy);
  assert(vec_enum[2] == standby);
  printf("v_vec_enum : %p [%d]\n", vec_enum, len[0]);

  double* vec2_real_base;
  fatptrToArray(v_2vec_real, (void**)&vec2_real_base, len, 2);
  double (*vec2_real)[len[0]] = (double(*)[len[0]])vec2_real_base;
  assert(vec2_real[0][0] == 0.1);
  assert(vec2_real[0][1] == 0.25);
  assert(vec2_real[0][2] == 0.5);
  assert(vec2_real[1][0] == 3.33);
  assert(vec2_real[1][1] == 4.25);
  assert(vec2_real[1][2] == 5.0);
  printf("v_2vec_real : %p [%d, %d]\n", vec_enum, len[1], len[0]);
}

fat_t* getString() {
  fat_t* fat = fatptrFromString("HELLO");
  printfat(fat);
  return fat;
}

fat_t* getIntVec() {
  int32_t vec[5] = {11, 22, 33, 44, 55};
  int32_t len[1] = {5};
  fat_t* fat = fatptrFromIntegerArray(vec, len, 1);
  printfat(fat);
  return fat;
}

line_t* getLine() {
  return lineFromString("HELLO WORLD");
}

extern int ghdl_main(int argc, char **argv);

int main(int argc, char **argv) {
  return ghdl_main(argc, argv);
}
