#ifndef ARBPRECISION_H
#define ARBPRECISION_H

#include "vector.h"

typedef struct bignm{
    vector a;
    int sign;
    int e;
} bignum;


void swap (bignum*, bignum*);
void init_bignum (bignum*);
void set_bignum (bignum*, bignum*);
void init_bignum_data (bignum*, vector*, int, int);
void cleanzeros (bignum*);
void free_bignum (bignum*);
void init_bignum_string (bignum*, char*);
void init_bignum_int (bignum*, int);
int iszero (bignum*);
void print_bignum_debug (bignum*);
bignum neg (bignum*);
int size_bignum (bignum*);
int lt (bignum*, bignum*);
int gt (bignum*, bignum*);
int le (bignum*, bignum*);
int ge (bignum*, bignum*);
int eq (bignum*, bignum*);
void add (bignum*, bignum*, bignum*);
void sub (bignum*, bignum*, bignum*);
void mul (bignum*, bignum*, bignum*);
void absol (bignum*, bignum*);
void divide (bignum*, bignum*, bignum*);
void expo (bignum*, bignum*, bignum*);
void sqt (bignum*, bignum*);
void intdiv2 (bignum*, bignum*);
void floatdiv2 (bignum*, bignum*);
void mulby1e9 (bignum*);
void divby1e9 (bignum*);
int mod2 (bignum*);
void trimto72dec (bignum*);
void trimto27dec (bignum*);
void print_bignum (bignum*);
void trim_self_20 (bignum*);

#endif
