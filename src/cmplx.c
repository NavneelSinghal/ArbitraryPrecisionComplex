#include <stdio.h>
#include <assert.h>
#include "arbprecision.h"
#include "cmplx.h"
#include <string.h>

#define DIV_BY_ZERO_CMPLX 0

int isreal(cmplx* num){
    return iszero(&num->im);
}

int isimag(cmplx* num){
    return iszero(&num->re);
}

void set_cmplx(cmplx* dest, cmplx* src){
    set_bignum(&(dest->re), &(src->re));
    set_bignum(&(dest->im), &(src->im));
}

void init_cmplx(cmplx* dest, bignum* re, bignum* im){
    set_bignum(&(dest->re), re);
    set_bignum(&(dest->im), im);
}

void add_cmplx(cmplx* dest, cmplx* src1, cmplx* src2){
    add(&dest->re, &src1->re, &src2->re);
    add(&dest->im, &src1->im, &src2->im);
}

void sub_cmplx(cmplx* dest, cmplx* src1, cmplx* src2){
    sub(&dest->re, &src1->re, &src2->re);
    sub(&dest->im, &src1->im, &src2->im);
}

void mul_cmplx(cmplx* dest, cmplx* src1, cmplx* src2){
    bignum ac, bd, ad, bc;
    mul(&ac, &src1->re, &src2->re);
    mul(&bd, &src1->im, &src2->im);
    mul(&ad, &src1->re, &src2->im);
    mul(&bc, &src1->im, &src2->re);
    bignum real, imag;
    sub(&real, &ac, &bd);
    add(&imag, &ad, &bc);
    init_cmplx(dest, &real, &imag);
    free_bignum(&real);
    free_bignum(&imag);
    free_bignum(&ac);
    free_bignum(&bd);
    free_bignum(&ad);
    free_bignum(&bc);
}

void div_cmplx(cmplx* dest, cmplx* src1, cmplx* src2){
    int isr = isreal(src2);
    int ism = isimag(src2);
    if(isr && ism){
        assert(DIV_BY_ZERO_CMPLX);
        return;
    }
    if(isr){
        bignum real, imag;
        divide(&real, &src1->re, &src2->re);
        divide(&imag, &src1->im, &src2->re);
        init_cmplx(dest, &real, &imag);
        free_bignum(&real);
        free_bignum(&imag);
        return;
    }
    if(ism){
        bignum real, imag, negimag;
        divide(&real, &src1->im, &src2->im);
        divide(&imag, &src1->re, &src2->im);
        negimag = neg(&imag);
        init_cmplx(dest, &real, &negimag);
        free_bignum(&real);
        free_bignum(&imag);
        free_bignum(&negimag);
        return;
    }
    bignum ac, bd, ad, bc, real, imag, realnorm, imagnorm, sqre, sqim, norm;
    mul(&ac, &src1->re, &src2->re);
    mul(&bd, &src1->im, &src2->im);
    mul(&ad, &src1->re, &src2->im);
    mul(&bc, &src1->im, &src2->re);
    add(&real, &ac, &bd);
    sub(&imag, &bc, &ad);
    mul(&sqre, &src2->re, &src2->re);
    mul(&sqim, &src2->im, &src2->im);
    add(&norm, &sqre, &sqim);
    divide(&realnorm, &real, &norm);
    divide(&imagnorm, &imag, &norm);
    init_cmplx(dest, &realnorm, &imagnorm);
    free_bignum(&real);
    free_bignum(&imag);
    free_bignum(&ac);
    free_bignum(&bd);
    free_bignum(&ad);
    free_bignum(&bc);
    free_bignum(&realnorm);
    free_bignum(&imagnorm);
    free_bignum(&sqre);
    free_bignum(&sqim);
    free_bignum(&norm);
}

void conj_cmplx(cmplx* dest, cmplx* src){
    bignum real, imag;
    set_bignum(&real, &src->re);
    imag = neg(&src->im);
    init_cmplx(dest, &real, &imag);
    free_bignum(&real);
    free_bignum(&imag);
}

void abs_cmplx(cmplx* dest, cmplx* src){
    bignum sqre, sqim, norm, zero, sqroot;
    mul(&sqre, &src->re, &src->re);
    mul(&sqim, &src->im, &src->im);
    add(&norm, &sqre, &sqim);
    init_bignum_int(&zero, 0);
    sqt(&sqroot, &norm);
    init_cmplx(dest, &sqroot, &zero);
    free_bignum(&sqre);
    free_bignum(&sqim);
    free_bignum(&norm);
    free_bignum(&zero);
    free_bignum(&sqroot);
}

void print_cmplx(cmplx* num){
    printf("(");
    print_bignum(&num->re);
    printf(",");
    print_bignum(&num->im);
    printf(")");
}

void free_cmplx(cmplx* num){
	free_bignum(&(num->re));
	free_bignum(&(num->im));
}
