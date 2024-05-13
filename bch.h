#ifndef BCH_H
#define BCH_H
#include <math.h>
#include <stdio.h>

void read_p(int M,int len);
void generate_gf();
void gen_poly(int T);

int decode_bch(unsigned char *recd);
void encode_bch(unsigned char *bb,unsigned char *data);
int get_K_value();


#endif