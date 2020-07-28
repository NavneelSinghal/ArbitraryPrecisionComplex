#include <stdio.h>
#include "arbprecision.h"
#include "cmplx.h"
#include <complex.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define NULL_STRING 0

char* get_next_line(FILE* f){
	char* ret = NULL;
	char c;
	int size = 0;
	int len = 0;
	while((c = fgetc(f)) != EOF && (c != '\n') && (c != '\r')){
		if(len == size){
			ret = realloc(ret, 2*size + 2);
			size = 2*size + 1;
		}
		ret[len] = c;
		len++;
	}
	if(c == '\r') fgetc(f);
	if(len != 0) {
		ret[len] = '\0';
		return ret;
	}
	else{
		return NULL;
	}
}

double atofnew(char* a){
	int sign = 1;
	if(*a == '-'){
		sign = -1;
		++a;
	}
	double number = 0;
	while(*a != '\0' && *a != '.'){
		number *= 10;
		number += *a - '0';
		++a;
	}
	if(*a == '.'){
		double p = 0.1;
		while(*a != '\0'){
			number += p * (*a - '0');
			p /= 10;
			++a;
		}
	}
	number *= sign;
	return number;
}

int main(int argc, char* argv[]){

	FILE* f = fopen(argv[1], "r");
	freopen(argv[2], "w", stdout);
	FILE* data = fopen("data.dat", "w");

    double times[10];
    int i;
    for(i = 0; i < 10; i++) times[i] = 0;

	char* line = get_next_line(f);
	float place = 0;
    while(line != NULL){

        double r1, i1, r2, i2;
		bignum re1, re2, re, im1, im2, im;
		cmplx c, c1, c2, res;
		clock_t start, end;
		double time_lib, time_own;
		double complex num1, num2, answer, num;
		char* s = strtok(line, " ,()");		
		if(strcmp(s, "ADD") == 0){
			s = strtok(NULL, " ,()");
            r1 = atofnew(s);
			init_bignum_string(&re1, s);
			s = strtok(NULL, " ,()");
			i1 = atofnew(s);
			init_bignum_string(&im1, s);
			s = strtok(NULL, " ,()");
			r2 = atofnew(s);
			init_bignum_string(&re2, s);
			s = strtok(NULL, " ,()");
			i2 = atofnew(s);
			init_bignum_string(&im2, s);
			init_cmplx(&c1, &re1, &im1);
			init_cmplx(&c2, &re2, &im2);
			start = clock();
			add_cmplx(&res, &c1, &c2);
			end = clock();
			time_own = ((double) (end - start)) / CLOCKS_PER_SEC;
			num1 = (r1 + I * i1);
			num2 = (r2 + I * i2);
			start = clock();
			answer = num1 + num2; /* */
			end = clock();
			time_lib = ((double) (end - start)) / CLOCKS_PER_SEC;
			print_cmplx(&res);
			printf("\n");
			free_bignum(&re1);
			free_bignum(&im1);
			free_bignum(&re2);
			free_bignum(&im2);
			free_cmplx(&c1);
			free_cmplx(&c2);
			free_cmplx(&res);
			times[0] += time_lib;
            times[1] += time_own;
		}
		else if(strcmp(s, "SUB") == 0){
			s = strtok(NULL, " ,()");
			init_bignum_string(&re1, s);
			r1 = atofnew(s);
			s = strtok(NULL, " ,()");
			i1 = atofnew(s);
			init_bignum_string(&im1, s);
			s = strtok(NULL, " ,()");
			r2 = atofnew(s);
			init_bignum_string(&re2, s);
			s = strtok(NULL, " ,()");
			i2 = atofnew(s);
			init_bignum_string(&im2, s);
			init_cmplx(&c1, &re1, &im1);
			init_cmplx(&c2, &re2, &im2);
			start = clock();
			sub_cmplx(&res, &c1, &c2);
			end = clock();
			time_own = ((double) (end - start)) / CLOCKS_PER_SEC;
			num1 = (r1 + I * i1);
			num2 = (r2 + I * i2);
			start = clock();
			answer = num1 - num2; /* */
			end = clock();
			time_lib = ((double) (end - start)) / CLOCKS_PER_SEC;
			print_cmplx(&res);
			printf("\n");
			free_bignum(&re1);
			free_bignum(&im1);
			free_bignum(&re2);
			free_bignum(&im2);
			free_cmplx(&c1);
			free_cmplx(&c2);
			free_cmplx(&res);
            times[2] += time_lib;
            times[3] += time_own;
		}
		else if(strcmp(s, "PROD") == 0){
			s = strtok(NULL, " ,()");
			init_bignum_string(&re1, s);
			r1 = atofnew(s);
			s = strtok(NULL, " ,()");
			i1 = atofnew(s);
			init_bignum_string(&im1, s);
			s = strtok(NULL, " ,()");
			r2 = atofnew(s);
			init_bignum_string(&re2, s);
			s = strtok(NULL, " ,()");
			i2 = atofnew(s);
			init_bignum_string(&im2, s);
			init_cmplx(&c1, &re1, &im1);
			init_cmplx(&c2, &re2, &im2);
			start = clock();
			mul_cmplx(&res, &c1, &c2);
			end = clock();
			time_own = ((double) (end - start)) / CLOCKS_PER_SEC;
			num1 = (r1 + I * i1);
			num2 = (r2 + I * i2);
			start = clock();
			answer = num1 * num2; /* */
			end = clock();
			time_lib = ((double) (end - start)) / CLOCKS_PER_SEC;
			print_cmplx(&res);
			printf("\n");
			free_bignum(&re1);
			free_bignum(&im1);
			free_bignum(&re2);
			free_bignum(&im2);
			free_cmplx(&c1);
			free_cmplx(&c2);
			free_cmplx(&res);
            times[4] += time_lib;
            times[5] += time_own;
		}
		else if(strcmp(s, "QUOT") == 0){
			s = strtok(NULL, " ,()");
			init_bignum_string(&re1, s);
			r1 = atofnew(s);
			s = strtok(NULL, " ,()");
			i1 = atofnew(s);
			init_bignum_string(&im1, s);
			s = strtok(NULL, " ,()");
			r2 = atofnew(s);
			init_bignum_string(&re2, s);
			s = strtok(NULL, " ,()");
			i2 = atofnew(s);
			init_bignum_string(&im2, s);
			init_cmplx(&c1, &re1, &im1);
			init_cmplx(&c2, &re2, &im2);
			start = clock();
			div_cmplx(&res, &c1, &c2);
			end = clock();
			time_own = ((double) (end - start)) / CLOCKS_PER_SEC;
			num1 = (r1 + I * i1);
			num2 = (r2 + I * i2);
			start = clock();
			answer = num1 / num2; /* */
			end = clock();
			time_lib = ((double) (end - start)) / CLOCKS_PER_SEC;
			print_cmplx(&res);
			printf("\n");
			free_bignum(&re1);
			free_bignum(&im1);
			free_bignum(&re2);
			free_bignum(&im2);
			free_cmplx(&c1);
			free_cmplx(&c2);
			free_cmplx(&res);
            times[6] += time_lib;
            times[7] += time_own;
		}
		else if(strcmp(s, "ABS") == 0){
			s = strtok(NULL, " ,()");
			init_bignum_string(&re1, s);
			r1 = atofnew(s);
			s = strtok(NULL, " ,()");
			i1 = atofnew(s);
			init_bignum_string(&im1, s);
			init_cmplx(&c1, &re1, &im1);
			start = clock();
			abs_cmplx(&res, &c1);
			end = clock();
			time_own = ((double) (end - start)) / CLOCKS_PER_SEC;
			num1 = (r1 + I * i1);
			start = clock();
			double absanswer = cabs(num1); 
			end = clock();
			time_lib = ((double) (end - start)) / CLOCKS_PER_SEC;
			print_cmplx(&res);
			printf("\n");
			free_bignum(&re1);
			free_bignum(&im1);
			free_cmplx(&c1);
			free_cmplx(&res);
            times[8] += time_lib;
            times[9] += time_own;
		}
        /*fprintf(data, "%f\t%f\n", place, time_lib);
        place += 0.5;
        fprintf(data, "%f\t%f\n\n", place, time_own);
        place += 1;*/
		free(line);
		line = get_next_line(f);
	}

    for(i = 0; i < 10; i+=2){
        fprintf(data, "%f\t%f\n", place, times[i]);
        place += 0.5;
        fprintf(data, "%f\t%f\n\n", place, times[i+1]);
        place += 1;
    }

    fclose(data);

    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set xtics (\"ADD\" 0.25, \"SUB\" 1.75, \"MUL\" 3.25, \"QUOT\" 4.75, \"ABS\" 6.25,); set boxwidth 0.5; plot 'data.dat' every 2 using 1:2 title \"C implementation using double\" with boxes ls 1, 'data.dat' every 2::1 using 1:2 title \"My implementation\" with     boxes ls 2; set term png; set output '%s'; replot\n", argv[3]);
	fclose(gnuplotPipe);
    fclose(f);
	return 0;
}
