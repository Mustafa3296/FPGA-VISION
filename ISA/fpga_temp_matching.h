#include <stdlib.h>
#include <stdio.h>
#include "ap_int.h"


// size of (tmphei * tmpwid + inhei * inwid) = (100x100 + 200x200)
#define C_NUM_COEFF 9
#define innum  50000 
#define size   40000 // inhei * inwid
#define inhei  200
#define inwid  200
#define tmphei 100
#define tmpwid 100
#define thre   30000

struct axis_t {
    int data;
    ap_int<1> last;
};

void top(axis_t *INPUT, axis_t *OUTPUT,
    int coeff[C_NUM_COEFF], unsigned int length)

void gray(int in[innum], int out[innum]);

void match(int in[innum/*size*/], int out[size]);

void similar (int win[tmphei][tmpwid], 
    int tmpgray[tmphei][tmpwid], int *out);


