#ifndef CMPLX_H
#define CMPLX_H

#include "arbprecision.h"

typedef struct cmpl{
    bignum re;
    bignum im;
} cmplx;

void add_cmplx (cmplx*, cmplx*, cmplx*);
void sub_cmplx (cmplx*, cmplx*, cmplx*);
void mul_cmplx (cmplx*, cmplx*, cmplx*);
void div_cmplx (cmplx*, cmplx*, cmplx*);
void abs_cmplx (cmplx*, cmplx*);
void print_cmplx (cmplx*);
void init_cmplx (cmplx*, bignum*, bignum*);
void free_cmplx (cmplx*);

#endif
