#include "fpga_temp_matching.h"


void top(axis_t *INPUT, axis_t *OUTPUT,
    int coeff[C_NUM_COEFF], unsigned int length){

#pragma HLS INTERFACE axis depth=50 port=INPUT
#pragma HLS INTERFACE axis depth=50 port=OUTPUT


#pragma HLS INTERFACE s_axilite port=coeff  bundle=CTRL
#pragma HLS INTERFACE s_axilite port=length bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
#pragma HLS ARRAY_PARTITION COMPLETE variable=coeff


        int window[C_NUM_COEFF] = {0};
#pragma HLS ARRAY_PARTITION COMPLETE variable=window

        int sum;
        axis_t cur;


	char intmp[200000/*innum*/];
	int outg[size];

	for (int i = 0; i < 200000/*length*/; i++) {

        cur = *INPUT++;
		intmp[i] = cur.data;

	}

    int ingray[innum];   // intmp contains both source img and template
    gray((int*) intmp, ingray); // grayscale source and template
    match(ingray, outg); // perform template matching

    /*for (int i = 0; i < size; i++) {
        printf("%d\n", outg[i]);
    }*/

    cur.last = 0;
    for (int i = 0; i < size; i++) {
        cur.data = outg[i];
    	if (i == size - 1) {
    		cur.last = 1;
    	} else cur.last = 0;
    	*OUTPUT++ = cur;
    }

}

void gray(int in[innum], int out[innum]) {

    int i, pix, y; //,simi;
    unsigned char r, g, b;
    for (i = 0; i < innum; i++) {
        pix = in[i];
        r = pix;
        g = pix >> 8;
        b = pix >> 16;
        y = 19595*r + 39470*g + 7471*b;
        out[i] = y >> 16;
    }
}

void match(int in[innum/*size*/], int out[size]) {

    int tp[tmphei][tmpwid]; // template image
    int i, j, k, l, ptr, num = 0, pix, simi = 0;
    static int buf[tmphei][inwid]; // buffer
    static int win[tmphei][tmpwid]; // window
    static int pixel[tmphei];

    // parse template image
    for (i=0; i<tmphei; i++) {
        for(j = 0; j < tmpwid; j++) {
            tp[i][j] = in[num++];
        }
    }

    for (i = 0; i < inhei; i++) {
        for (j = 0; j < inwid; j++) {
            pix = in[num];
            for (k = 0; k < tmphei-1; k++) {
                buf[k][j] = buf[k+1][j];
                pixel[k] = buf[k][j];
            }
            pixel[tmpwid - 1] = buf[tmpwid - 1][j] = pix;
            for (k = 0; k < tmphei; k++) {
                for(l = 0; l < tmpwid-1; l++) {
                    win[k][l] = win[k][l+1];
                }
            }
            for (k = 0; k < tmphei; k++) {
                win[k][tmpwid-1] = pixel[k];
            }

            similar(win, tp, /*tmpgray,*/ &simi);
            //printf("%d\n", simi);
            if (simi <= thre) { out[num - tmpwid*tmphei] = 1; } else { out[num - tmpwid*tmphei] = 0; }
            num++;
        }
    }
}

void similar(int win[tmphei][tmpwid],
    int tmpgray[tmphei][tmpwid], int *out) {

    int i, j, absl, sad = 0; // SAD (Sum of Absolute Differences)
    for (i = 0; i < tmphei; i++) {
        for (j = 0; j < tmpwid; j++) {
            //printf("win[i][j]: %d, tmp[i][j]: %d\n", win[i][j], tmpgray[i][j]);
            absl = abs(win[i][j] - tmpgray[i][j]);
            /*if (!absl) {
                printf("%d\n", absl);
            }*/
            sad+= absl;
        }
    }
    *out = sad;
    //printf("%d\n", *out);
}
