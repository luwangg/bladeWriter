#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <libbladeRF.h>


#define ITERATIONS 100000
#define BASE_FREQUENCY 3100000000
#define CHAR_HEIGHT 11

void sig_handler(int sig);
void genTable(int argc, char *args[]);
void defaultTable();
//struct bladerf *dev = NULL;
struct freqGroup *table;


unsigned int checkStatus(int status);

#define ROWS 11
#define COLS 11
const unsigned int waitRow = 100;

//We've got base freq, decade, and val
struct freqGroup {
    float startFreq; // Freq to start table at
    float endFreq;   // Freq to end table at
    unsigned int colWidth;  // col width is horizontal resolution.
    unsigned int rowHeight;

    unsigned int decade;    // The frequency family to transmit at [Mhz or Ghz]
    unsigned int limit;     // a state variable. Will probably be removed.

    unsigned int *freqTable;
};

unsigned int frequencies[COLS] = {
  902000000,
  902010000,
  902020000,
  902030000,
  902040000,
  902050000,
  902060000,
  902070000,
  902080000,
  902090000,
  902100000
};


/*
unsigned int frequencies[NUM_FREQUENCIES] = {
  902000000,
  902100000,
  902200000,
  902300000,
  902400000,
  902500000,
  902500000,
  902600000,
  902700000,
  902800000,
  902900000
};
*/

unsigned int csh_logo[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,0,0,1},
    {1,0,0,0,0,1,0,1,0,0,1},
    {1,0,0,0,0,1,0,1,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1},
    {1,0,1,0,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,0,1,0,0,1},
    {1,0,1,1,1,1,0,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,0,1}
};


