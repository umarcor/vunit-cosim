/*
Mixed-Signal (Digital/Analog) simulation with GHDL and Xyce through VHPIDIRECT
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <N_CIR_XyceCInterface.h>

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

uint32_t xyce(fat_t* ptr) {
    assert(ptr != NULL);
    assert(ptr->bounds != NULL);
    int len = ptr->bounds->length;
    char* cir = malloc(sizeof(char) * len + 1);
    strncpy(cir, ptr->base, len);
    cir[len] = '\0';

    printf("VHPIDIRECT: entering xyce %s\n", cir);

    void** p = (void **) malloc( sizeof(void* [1]) );

    char *argList[] = {
        (char*)("Xyce"),
        (char*)("-quiet"),
        //(char*)("-o"),
        //(char*)("testOutput"),
        (char*)(cir),
    };

    int status;

    xyce_open(p);

    status = xyce_initialize(p, sizeof(argList)/sizeof(argList[0]), argList);
    assert( status == 1);
    assert( p != NULL );

    status = xyce_runSimulation(p);
    assert( status == 1);

    xyce_close(p);
    free(p);

    printf("VHPIDIRECT: exiting xyce\n");
    return status;
}
