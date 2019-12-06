#include <assert.h>
#include <string.h>

#include <N_CIR_XyceCInterface.h>

#include "vhpidirect_xyce_storage.h"
#include "ghdl.h"

//--

extern int ghdl_main (int argc, char **argv);

//--

uint32_t _configDACs(xhdl_t* item) {
  // Get the number of YADC and YDAC in the netlist, and maximum name lengths
  // for each type of device. This allows for better sizing of the ADCnames
  // and DACnames char arrays.
  int maxADCnameLength = 0;  int* maxADCnameLengthPtr = &maxADCnameLength;
  int maxDACnameLength = 0;  int* maxDACnameLengthPtr = &maxDACnameLength;

  item->ADC->num = malloc(sizeof(uint));
  item->DAC->num = malloc(sizeof(uint));

  xyce_getNumDevices(item->ptr, (char *)"YADC", item->ADC->num, maxADCnameLengthPtr);
  xyce_getNumDevices(item->ptr, (char *)"YDAC", item->DAC->num, maxDACnameLengthPtr);

  if (*(item->ADC->num) == 0 && *(item->DAC->num) == 0) {
    printf("_configDACs: No ADCs or DACs found!\n");
    return 0;
  }

  printf("_configDACs: ADC names [num , max length]: %d %d\n", *(item->ADC->num), maxADCnameLength);
  printf("_configDACs: DAC names [num , max length]: %d %d\n", *(item->DAC->num), maxDACnameLength);

  // Initialize arrays of char array

  item->ADC->names = (char **) malloc( *(item->ADC->num) * sizeof(char*) );
  item->DAC->names = (char **) malloc( *(item->DAC->num) * sizeof(char*) );

  int i; // loop counter
  for (i = 0; i < *(item->ADC->num); i++) {
    item->ADC->names[i] = (char *) malloc( maxADCnameLength*sizeof(char) );
  }
  for (i = 0; i < *(item->DAC->num); i++) {
    item->DAC->names[i] = (char *) malloc( maxDACnameLength*sizeof(char) );
  }

  xyce_getDeviceNames(item->ptr, (char *)"YADC", item->ADC->num, item->ADC->names);
  xyce_getDACDeviceNames(item->ptr, item->DAC->num, item->DAC->names);

  printf("Found %d ADC devices\n",*(item->ADC->num));
  for (i = 0; i < *(item->ADC->num); i++) {
    printf("ADC Name %d: %s\n", i, item->ADC->names[i]);
  }

  printf("Found %d DACs\n",*(item->DAC->num));
  for (i = 0; i < *(item->DAC->num); i++) {
    printf("DAC Name %d: %s\n", i, item->DAC->names[i]);
  }
}


uint32_t xhdl_init(char **id, char **circuit) {
  int z = st_get_free();
  assert( z >= 0 );
  printf("DEBUG: xhdl_init <%s> [%d]\n", *id, z);

  void** p = (void **) malloc( sizeof(void* [1]) );

  xhdl_db[z] = new_xhdl(*id, p);

  char *argList[] = {
      (char*)("Xyce"),
      //(char*)("-quiet"),
      //(char*)("-o"),
      //(char*)("testOutput"),
      (char*)(*circuit),
  };

  int status;

  xyce_open(p);

  status = xyce_initialize(p, sizeof(argList)/sizeof(argList[0]), argList);
  assert( status == 1);
  assert( p != NULL );

  _configDACs(xhdl_db[z]);

  // A bug in the DAC device (put there for Habinero support) only takes
  // the last time in the time voltage pairs list if the current sim time is 0.
  // So simulate a bit first.
  status = xyce_simulateUntil(p, 1.0e-10, xhdl_db[z]->time );
  assert(status == 1);

  return status;
}


uint32_t xhdl_run(char **id, double requestedUntilTime, fat_t* tArray, fat_t* vArray) {

  uint z = st_find(*id);
  assert(z >= 0);
  printf("DEBUG: xhdl_run <%s> [%d] <%f> <%p> <%p>\n", *id, z, requestedUntilTime, tArray, vArray);

  void** p = xhdl_db[z]->ptr;
  assert(p != NULL);

  int status = 0;

  assert(tArray != NULL);
  assert(vArray != NULL);
  assert(tArray->bounds != NULL);
  assert(vArray->bounds != NULL);
  assert(tArray->bounds->length == vArray->bounds->length);

  uint numPoints = tArray->bounds->length;

  if ( numPoints != 0 ) {
    double* tArr = (double*) tArray->base;
    double* vArr = (double*) vArray->base;
    int y;
    for (y=0;y<numPoints;y++) {
      printf("[%d]: %f %f\n", y, tArr[y], vArr[y]);
    }

    status = xyce_updateTimeVoltagePairs(
      p,
      xhdl_db[z]->DAC->names[0],
      numPoints,
      tArr,
      vArr
    );
    assert( status == 1);
  }

  if ( requestedUntilTime != 0 ) {
    status = xyce_simulateUntil(p, requestedUntilTime, xhdl_db[z]->time );
  } else {
    status = xyce_runSimulation(p);
  }
  assert( status == 1);

  return status;
}


double xhdl_read(char **id, char** name) {
  uint z = st_find(*id);
  assert(z >= 0);
  printf("DEBUG: xhdl_run <%s> [%d] <%s>\n", *id, z, *name);

  void** p = xhdl_db[z]->ptr;
  assert(p != NULL);

  double* val = malloc(sizeof(double));
  xyce_obtainResponse(p, *name, val);
  printf( "Result = %f\n", *val);

  return *val;
}


void xhdl_close(char **id) {
  printf("DEBUG: xhdl_run <%s>\n", *id);

  uint z = st_find(*id);
  assert(z >= 0);

  xyce_close(xhdl_db[z]->ptr);

  st_free_item(z);
}
