#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "arbprecision.h"

#define VEC_SIZE 16
#define comment(x)
#define false 0
#define true 1
#define base 1000000000

const int NEGATIVE_NUM_SQRT = 0;
const int DIV_BY_0 = 0;
const int INDEX_OUT_OF_BOUNDS = 0;

/*
 * HELPER FUNCTIONS 
 */

int min(int a, int b){
	if(b < a) return b;
	else return a;
}

int max(int a, int b){
	if(b > a) return b;
	else return a;
}

/*
 * VECTOR IMPLEMENTATION
 */
/*
typedef struct vectr{
	int size;
	int *data;
	int capacity;
} vector;
*/
void init_vector(vector* v){
	v->capacity = VEC_SIZE;
	v->size = 0;
	v->data = (int*) malloc(v->capacity * sizeof(int));
	int i;
	for(i = 0; i < v->capacity; i++) (v->data)[i] = 0;
}

int size(vector* v){
	return v->size;
}

void resize(vector* v, int a){ /*assumes a is >= 0*/
	v->capacity = a;
	v->data = realloc(v->data, v->capacity * sizeof(int));
	int i;
	for(i = v->size; i < v->capacity; i++){
		(v->data)[i] = 0;
	}
	v->size = v->capacity;
}

void resize_internal(vector* v, int a){
	v->capacity = a;
	v->data = realloc(v->data, v->capacity * sizeof(int));
	int i;
	for(i = v->size; i < v->capacity; i++){
		(v->data)[i] = 0;
	}
	if(v->size > v->capacity) v->size = v->capacity;
}

void freevector(vector* v){
	v->size = 0;
	v->capacity = 0;
	free(v->data);
}

void erase(vector* v, int a){ /*assume that a < size of v*/
	if(a < v->size && a >= 0){
		int i;
		for(i = a; i < v->size - 1; ++i){
			(v->data)[i] = (v->data)[i + 1];
		}
		v->data[v->size - 1] = 0;
		--(v->size);
		if(v->size == 0){
			freevector(v);
		}
		if(v->capacity > 2*(v->size)){
			resize_internal(v, v->size);
		}
	}
}

void erase_range(vector* v, int l, int r){ 
	/*
	 *erases elements in range [l, r)
	 */
	if(l >= 0 && l < r && r <= v->size){
		int i = l;
		for(; i + r - l < v->size; i++){
			(v->data)[i] = (v->data)[i + r - l];
		}
		for(; i < v->size; i++){
			(v->data)[i] = 0;
		}
		v->size -= (r - l);
		if(v->size == 0){
			freevector(v);
		}
		if(v->capacity > 2*(v->size)){
			resize_internal(v, v->size);
		}
	}
}

void push_back(vector* v, int a){
	if(v->capacity == 0){
		init_vector(v);
	}
	if(v->size == v->capacity){
		resize_internal(v, 2*v->capacity);
	}
	(v->data)[v->size] = a;
	++(v->size);
}

void pop_back(vector* v){
	erase(v, v->size - 1);
}

int get(vector* v, int index){
	if(index < v->size && index >= 0){
		return (v->data)[index];
	}
	return -1;
}

void set(vector* v, int index, int value){
	if(index < v->size && index >= 0){
		(v->data)[index] = value;
		return;
	}
	assert(INDEX_OUT_OF_BOUNDS);
}

void copyvector(vector* a, vector* b){ /*copies b into a*/
	/*freevector(a);*/ /*check if this can be freed only if init has been called*/
	(a)->size = b->size;
	(a)->capacity = b->size;
	(a)->data = realloc(a->data, (a)->size * sizeof(int));
	int i;
	for(i = 0; i < (a)->size; i++){
		(a->data)[i] = (b->data)[i];
	}
}

int back(vector* a){
	return a->data[a->size-1];
}

void print_vector(vector* v){
	int i = 0;
	for(i = 0; i < v->size; i++){
		printf("%d ", get(v, i));
	}
	printf("\n");
}

void reverse(vector *v){
	int i = 0;
	int j = v->size - 1;
	while(i < j){
		int temp = get(v, i);
		set(v, i, get(v, j));
		set(v, j, temp);
		i++;
		j--;
	}
}

/*
 *BIGNUM IMPLEMENTATION
 */

/*typedef struct bignm{
	vector a;
	int sign;
	int e;
} bignum;*/

/*
void add(bignum*, bignum*, bignum*);
void sub(bignum*, bignum*, bignum*);
void mul(bignum*, bignum*, bignum*);
void print_bignum_debug(bignum*);
int mod2(bignum*);
void intdiv2(bignum*, bignum*);
void trim_self_20(bignum*);
void mulby1e9(bignum*);
void divby1e9(bignum*);
void floatdiv2(bignum*, bignum*);
*/

void swap(bignum* a, bignum* b){
	bignum temp = *a;
	*a = *b;
	*b = temp;
}

void init_bignum(bignum* num){
	num->sign = 1;
	num->e = 0;
	init_vector(&(num->a));
}

void init_bignum_data(bignum* num, vector* a, int sign, int e){ /*assumes num is empty initially*/
	init_vector(&(num->a));
	copyvector(&(num->a), a);
	num->sign = sign;
	num->e = e;
}

void cleanzeros(bignum* num){
	int i = 0;
	while(((num->e) > 0) && (i < ((num->a).size - 1))  && (get(&(num->a), i) == 0)) i++, (num->e)--;
	comment(printf("i and e are %d, %d resp\n", i, num->e));
	erase_range(&(num->a), 0, i);
	while((num->a).size > 1 && back(&(num->a)) == 0){
		pop_back(&(num->a));
	}
}

void free_bignum(bignum* num){
	freevector(&(num->a));
}

void init_bignum_string(bignum* num, char* s){
	init_vector(&(num->a));
	if(s[0] == '-'){
		num->sign = -1, s++;
	}
	else{
		num->sign = 1;
	}
	char *cc = s;
	while(*cc != '\0'){
		if(*cc == '.') break;
		cc++;
	}
	if(*cc == '\0') cc = NULL;
	int d = 0;
	if(cc == NULL){
		num->e = 0;
	}
	else{
		int f = (strlen(s) - 1 - (cc - s) + 8);
		num->e = f/9;
		d = -f + 9*(num->e) + 8;
	}
	vector w;
	init_vector(&w);
	int i;
	for(i = 0; i < d; i++) push_back(&w, 0);
	int len = strlen(s);
	for(i = len - 1; i >= 0; i--){
		if(s[i] == '.' || s[i] == '-') continue;
		else push_back(&w, s[i] - '0');
	}
	reverse(&w);
	i = w.size - 1;
	for(; i >= 0; i -= 9){
		int number = 0;
		int j = max(0, i - 8);
		for(; j <= i; j++){
			number *= 10;
			number += get(&w, j);
		}
		push_back(&(num->a), number);
	}
	freevector(&w);
	cleanzeros(num);
}

void init_bignum_int(bignum* num, int a){
	init_vector(&(num->a));
	push_back(&(num->a), a);
	if(a >= 0) num->sign = 1;
	else num->sign = -1;
	num->e = 0;
}

int iszero(bignum* num){
	return (((num->a).size == 1) && get(&(num->a), 0) == 0);
}

void print_bignum_debug(bignum* num){
	if(iszero(num)){
		printf("0");
		return;
	}
	if(num->sign == -1) printf("-");
	if((num->a).size > (num->e)){
		int choice = (num->e != 0);
		int i = -1 + size(&(num->a));
		for(; i >= 0; i--){
			int x = get(&(num->a), i);
			int dig = 0;
			while(x){
				x /= 10;
				++dig;
			}
			if(get(&(num->a), i) == 0) dig = 1;
			if(i != -1 + size(&(num->a))){
				int j = 0;
				for(; j < 9 - dig; j++){
					printf("0");
				}
			}
			if(!choice || i != 0){
				printf("%d", get(&(num->a), i));
			}
			else{
				int xa = get(&(num->a), i);
				while(xa % 10 == 0){
					xa /= 10;
				}
				printf("%d", xa);
			}
			if(num->e == i && choice) printf(".");
		}
	}
	else{
		int l = num->e - (num->a).size;
		printf("0.");
		int i = 0;
		for(; i < l; i++){
			printf("000000000");
		}
		for(i = (num->a).size - 1; i >= 0; i--){
			int x = get(&(num->a), i);
			int dig = 0;
			while(x){
				x /= 10;
				++dig;
			}
			if(get(&(num->a), i) == 0) dig = 1;
			int j = 0;
			for(; j < 9 - dig; j++){
				printf("0");
			}
			if(i != 0 || num->e == 0) printf("%d", get(&(num->a), i));
			else{
				int xa = get(&(num->a), i);
				while(xa % 10 == 0) xa /= 10;
				printf("%d", xa);
			}
		}
	}
}

bignum neg(bignum* a){
	bignum ret;
	init_bignum_data(&ret, &(a->a), -(a->sign), (a->e));
	return ret;
}

int size_bignum(bignum* num){
	return (num->a).size;
}

int lt(bignum *a, bignum *b){
	if(iszero(a) && iszero(b)) return false;
	if(iszero(a) && b->sign > 0) return true;
	if(a->sign > 0 && iszero(b)) return false;
	if(a->sign != b->sign) return a->sign < b->sign;
	if(a->sign == -1){
		bignum na = neg(a);
		bignum nb = neg(b);
		int ans = lt(&nb, &na);
		free_bignum(&na);
		free_bignum(&nb);
		return ans;
	}
	int diffa = a->a.size - a->e;
	int diffb = b->a.size - b->e;
	if(diffa < diffb) return true;
	else if(diffa > diffb) return false;
	else{
		int asize = a->a.size;
		int bsize = b->a.size;
		int minsize = min(asize, bsize);
		int i = 0;
		for(i = 0; i < minsize; i++){
			if(get(&(a->a), asize - i - 1) < get(&(b->a), bsize - i - 1)){
				return true;
			}
			else if(get(&(a->a), asize - i - 1) > get(&(b->a), bsize - i - 1)){
				return false;
			}
		}
		return asize < bsize;
	}
}

int gt(bignum* a, bignum* b){
	return lt(b, a);
}

int le(bignum* a, bignum* b){
	return !(gt(a, b));
}

int ge(bignum* a, bignum* b){
	return !(lt(a, b));
}

int eq(bignum* a, bignum* b){
	return le(a, b) && ge(a, b);
}

void set_bignum(bignum* dest, bignum* src){ /*also assumes dest is empty initially (i.e., init hasn't been done on dest)*/
	init_bignum_data(dest, &(src->a), src->sign, src->e);
}

void add(bignum* dest, bignum* num1, bignum* num2){
	bignum tempa, tempb;
	set_bignum(&tempa, num1);
	set_bignum(&tempb, num2);
	if(tempa.sign != tempb.sign){
		if(tempa.sign == -1){
			tempa.sign = 1;
			sub(dest, &tempb, &tempa);
		}
		else{
			tempb.sign = 1;
			sub(dest, &tempa, &tempb);
		}
		free_bignum(&tempa);
		free_bignum(&tempb);
		return;
	}
	
	int maxe = max(tempa.e, tempb.e);
	if(tempa.e != maxe){
		int diff = maxe - tempa.e;
		vector vec;
		init_vector(&vec);
		resize(&vec, diff);
		int tempasize = size(&(tempa.a));
		int i;
		for(i = 0; i < tempasize; i++){
			push_back(&vec, get(&(tempa.a), i));
		}
		freevector(&(tempa.a));
		init_vector(&(tempa.a));
		copyvector(&(tempa.a), &vec);
		freevector(&vec);
		tempa.e = maxe;
	}
	if(tempb.e != maxe){
		int diff = maxe - tempb.e;
		vector vec;
		init_vector(&vec);
		resize(&vec, diff);
		int tempbsize = size(&(tempb.a));
		int i;
		for(i = 0; i < tempbsize; i++){
			push_back(&vec, get(&(tempb.a), i));
		}
		freevector(&(tempb.a));
		init_vector(&(tempb.a));
		copyvector(&(tempb.a), &vec);
		freevector(&vec);
		tempb.e = maxe;
	}
	int maxsize = max(size(&(tempa.a)), size(&(tempb.a)));
	vector buf;
	init_vector(&buf);
	resize(&buf, maxsize);
	int asize = size(&(tempa.a));
	int bsize = size(&(tempb.a));
	int carry = 0;
	int i;
	for(i = 0; i < maxsize; i++){
		int sum = carry;
		if(i < asize) sum += get(&(tempa.a), i);
		if(i < bsize) sum += get(&(tempb.a), i);
		if(sum >= base) sum -= base, carry = 1;
		else carry = 0;
		set(&buf, i, sum);
	}
	if(carry != 0){
		push_back(&buf, carry);
	}
	init_bignum_data(dest, &buf, tempa.sign, maxe);
	cleanzeros(dest);
	freevector(&buf);
	free_bignum(&tempa);
	free_bignum(&tempb);
	return;
}

void sub(bignum* dest, bignum* num1, bignum* num2){
	bignum tempa, tempb;
	set_bignum(&tempa, num1);
	set_bignum(&tempb, num2);
	if(tempa.sign != tempb.sign){
		if(tempa.sign == 1){
			tempb.sign = 1;
			add(dest, &tempa, &tempb);
		}
		else{
			tempb.sign = -1;
			add(dest, &tempa, &tempb);
		}
		free_bignum(&tempa);
		free_bignum(&tempb);
		return;
	}
	if(tempa.sign == -1){
		bignum tempc = neg(&tempa);
		bignum tempd = neg(&tempb);
		sub(dest, &tempd, &tempc);
		free_bignum(&tempa);
		free_bignum(&tempb);
		free_bignum(&tempc);
		free_bignum(&tempd);
		return;
	}
	int sgn = 1;
	if(lt(&tempa, &tempb)){
		swap(&tempa, &tempb);
		sgn = -1;
	}
	int maxe = max(tempa.e, tempb.e);
	if(tempa.e != maxe){
		int diff = maxe - tempa.e;
		vector vec;
		init_vector(&vec);
		resize(&vec, diff);
		int tempasize = size(&(tempa.a));
		int i;
		for(i = 0; i < tempasize; i++){
			push_back(&vec, get(&(tempa.a), i));
		}
		freevector(&(tempa.a));
		init_vector(&(tempa.a));
		copyvector(&(tempa.a), &vec);
		freevector(&vec);
		tempa.e = maxe;
	}
	if(tempb.e != maxe){
		int diff = maxe - tempb.e;
		vector vec;
		init_vector(&vec);
		resize(&vec, diff);
		int tempbsize = size(&(tempb.a));
		int i;
		for(i = 0; i < tempbsize; i++){
			push_back(&vec, get(&(tempb.a), i));
		}
		freevector(&(tempb.a));
		init_vector(&(tempb.a));
		copyvector(&(tempb.a), &vec);
		freevector(&vec);
		tempb.e = maxe;
	}
	int maxsize = max(size(&(tempa.a)), size(&(tempb.a)));
	vector buf;
	init_vector(&buf);
	resize(&buf, maxsize);
	int asize = size(&(tempa.a));
	int bsize = size(&(tempb.a));
	int borrow = 0;
	int i;
	for(i = 0; i < maxsize; i++){
		int diff = borrow;
		if(i < asize) diff += get(&(tempa.a), i);
		if(i < bsize) diff -= get(&(tempb.a), i);
		if(diff < 0){
			diff += base;
			borrow = -1;
		}
		else{
			borrow = 0;
		}
		set(&buf, i, diff);
	}
	while(back(&buf) == 0 && size(&buf) > 1) pop_back(&buf);
	init_bignum_data(dest, &buf, sgn, maxe);
	cleanzeros(dest);
	freevector(&buf);
	free_bignum(&tempa);
	free_bignum(&tempb);
	return;
}

void mul(bignum* dest, bignum* num1, bignum* num2){
	if(iszero(num1) || iszero(num2)){
		init_bignum_int(dest, 0);
		return;
	}
	vector buf;
	init_vector(&buf);
	int asize = size(&(num1->a));
	int bsize = size(&(num2->a));
	resize(&buf, asize + bsize);
	int i = 0;
	for(i = 0; i < asize; i++){
		long long int carry = 0;
		int j = 0;
		for(j = 0; j < bsize; j++){
			long long int x = carry + get(&buf, i + j) + (long long int) get(&(num1->a), i) * get(&(num2->a), j);
			set(&buf, i + j, x % base);
			carry = x / base;
		}
		if(carry){
			set(&buf, i + j, carry);
		}
	}
	if(back(&buf) == 0){
		pop_back(&buf);
	}
	int d = num1->e + num2->e;
	init_bignum_data(dest, &buf, num1->sign * num2->sign, d);
	cleanzeros(dest);
	freevector(&buf);
}

void absol(bignum* dest, bignum* src){
	set_bignum(dest, src);
	dest->sign = 1;
}

void divide(bignum* dest, bignum* num1, bignum* num2){
	if(iszero(num2)){
		assert(DIV_BY_0);
		return;
	}
	if(iszero(num1)){
		init_bignum_int(dest, 0);
		return;
	}
	int it = 0;
	for(it = 0; it < 3; it++){
		mulby1e9(num1);
	}
	int expa = num1->e;
	int expb = num2->e;
	int diff = expa - expb;
	int finale = diff + 3;
	int i = 0;
	bignum tempa, tempb;
	vector va, vb;
	init_vector(&va);
	init_vector(&vb);
	resize(&va, max(0, -diff));
	resize(&vb, max(0, diff));
	if(va.size == 0) freevector(&va);
	if(vb.size == 0) freevector(&vb);
	for(i = 0; i < num1->a.size; i++){
		push_back(&va, get(&(num1->a), i));
	}
	for(i = 0; i < num2->a.size; i++){
		push_back(&vb, get(&(num2->a), i));
	}
	for(it = 0; it < 3; it++){
		divby1e9(num1);
	}
	init_bignum_data(&tempa, &va, 1, 0);
	init_bignum_data(&tempb, &vb, 1, 0);
	freevector(&va);
	freevector(&vb);
	int sign = (num1->sign) * (num2->sign);
	bignum zero;
	init_bignum_int(&zero, 0);
	init_bignum_int(dest, 0);
	bignum rem;
	init_bignum_int(&rem, 0);
	int norm = base / (1 + back(&(tempb.a)));
	bignum nrm;
	init_bignum_int(&nrm, norm);
	bignum bas;
	init_bignum_int(&bas, base);
	bignum a;
	bignum b;
	mul(&a, &tempa, &nrm);
	mul(&b, &tempb, &nrm);
	resize(&(dest->a), a.a.size);
	for(i = a.a.size - 1; i >= 0; i--){
		mulby1e9(&rem);
		bignum temp;
		init_bignum_int(&temp, a.a.data[i]);
		bignum temp2;
		add(&temp2, &temp, &rem);
		free_bignum(&temp);
		free_bignum(&rem);
		set_bignum(&rem, &temp2);
		free_bignum(&temp2);
		int left = 0;
		int right = base - 1;
		int ans = 0;
		while(left <= right){
			int mid = left + (right - left) / 2;
			bignum temp;
			init_bignum_int(&temp, mid);
			bignum product;
			mul(&product, &b, &temp);
			if(le(&product, &rem)){
				ans = mid;
				left = mid + 1;
			}
			else{
				right = mid - 1;
			}
			free_bignum(&product);
			free_bignum(&temp);
		}
		init_bignum_int(&temp, ans);
		bignum product;
		mul(&product, &b, &temp);
		bignum temprem;
		sub(&temprem, &rem, &product);
		free_bignum(&temp);
		free_bignum(&product);
		free_bignum(&rem);
		set_bignum(&rem, &temprem);
		free_bignum(&temprem);
		set(&(dest->a), i, ans);
	}
	dest->sign = sign;
	dest->e = finale;
	cleanzeros(dest);
	free_bignum(&tempa);
	free_bignum(&tempb);
	free_bignum(&zero);
	free_bignum(&nrm);
	free_bignum(&a);
	free_bignum(&b);
	free_bignum(&bas);
	free_bignum(&rem);
	trim_self_20(dest);
}

void expo(bignum* dest, bignum* src1, bignum* src2){
	if(src2->e != 0){
		init_bignum_int(dest, 0);
		return;
	}
	bignum res, b_, w;
	init_bignum_int(&res, 1);
	set_bignum(&b_, src2);
	set_bignum(&w, src1);
	while(!iszero(&b_)){
		if(mod2(&b_)){
			bignum product;
			mul(&product, &res, &w);
			free_bignum(&res);
			set_bignum(&res, &product);
			free_bignum(&product);
		}
		bignum divby2;
		intdiv2(&divby2, &b_);
		free_bignum(&b_);
		set_bignum(&b_, &divby2);
		free_bignum(&divby2);
		bignum square;
		mul(&square, &w, &w);
		free_bignum(&w);
		set_bignum(&w, &square);
		free_bignum(&square);
	}
	if(src2->sign == -1){
		bignum one;
		init_bignum_int(&one, 1);
		divide(dest, &one, &res);
		free_bignum(&one);
	}
	else{
		set_bignum(dest, &res);
	}
	free_bignum(&res);
	free_bignum(&w);
	free_bignum(&b_);
	return;
}

void oldsqt(bignum* dest, bignum* src){
	if(iszero(src)){
		init_bignum_int(dest, 0);
		return;
	}
	if(src->sign < 0){
		printf("Square root of negative number attempted\n");
		assert(NEGATIVE_NUM_SQRT);
		return;
	}
	bignum left, right, target, eps, one, adding, diff;
	int i;
	init_bignum_int(&one, 1);
	init_bignum_int(&eps, 1);
	for(i = 0; i < 7; i++) divby1e9(&eps);
	init_bignum_int(&adding, 1);
	for(i = 0; i < 6; i++) divby1e9(&adding);
	init_bignum_int(&left, 0);
	add(&right, &adding, src);
	set_bignum(&target, &right);
	if(lt(&right, &one)){
		free_bignum(&right);
		init_bignum_int(&right, 1);
	}
	init_bignum_int(dest, 0);
	sub(&diff, &right, &left);
	while(gt(&diff, &eps)){
		bignum mid;
		add(&mid, &left, &right);
		bignum temp;
		floatdiv2(&temp, &mid);
		swap(&temp, &mid);
		bignum sq;
		mul(&sq, &mid, &mid);
		if(le(&sq, &target)){
			free_bignum(dest);
			set_bignum(dest, &mid);
			free_bignum(&left);
			set_bignum(&left, &mid);
		}
		else{
			free_bignum(&right);
			set_bignum(&right, &mid);
		}
		free_bignum(&mid);
		free_bignum(&temp);
		free_bignum(&sq);
		free_bignum(&diff);
		sub(&diff, &right, &left);
	}
	bignum temp;
	add(&temp, dest, &eps);
	free_bignum(dest);
	set_bignum(dest, &temp);
	free_bignum(&temp);
	free_bignum(&left);
	free_bignum(&right);
	free_bignum(&target);
	free_bignum(&eps);
	free_bignum(&one);
	free_bignum(&adding);
	free_bignum(&diff);
	trim_self_20(dest);
}

void sqt(bignum* dest, bignum* src){
	if(src->sign < 0){
		assert(NEGATIVE_NUM_SQRT);
		return;
	}
	int i = 0;
	for(i = 0; i < 6; i++){
		mulby1e9(src);
	}
	bignum one;
	init_bignum_int(&one, 1);
	if(lt(src, &one)){
		init_bignum_int(dest, 0);
		for(i = 0; i < 6; i++){
			divby1e9(src);
		}
		free_bignum(&one);
		return;
	}
	free_bignum(&one);
	if((src->a.size - src->e) % 2 != 0){
		push_back(&(src->a), 0);
	}
	
	bignum rem;
	init_bignum_int(&rem, 0);
	bignum two;
	init_bignum_int(&two, 2);
	init_bignum_int(dest, 0);
	for(i = (src->a).size - 1; i > (src->e); i-=2){
		int big = (src->a).data[i];
		int small = (src->a).data[i-1];
		bignum b, s;
		init_bignum_int(&b, big);
		init_bignum_int(&s, small);
		bignum addend;
		mulby1e9(&b);
		add(&addend, &b, &s);
		free_bignum(&b);
		free_bignum(&s);
		
		bignum remnew;
		mulby1e9(&rem);
		mulby1e9(&rem);
		add(&remnew, &rem, &addend);
		swap(&rem, &remnew);
		free_bignum(&remnew);
		free_bignum(&addend);
		
		mulby1e9(dest);
		bignum x;
		mul(&x, dest, &two);
		int left = 0;
		int right = base - 1;
		int ans = 0;
		while(left <= right){
			int mid = left + (right - left)/2;
			bignum w;
			init_bignum_int(&w, mid);
			bignum sum;
			add(&sum, &x, &w);
			bignum product;
			mul(&product, &sum, &w);
			free_bignum(&sum);
			free_bignum(&w);
			if(le(&product, &rem)){
				left = mid + 1;
				ans = mid;
			}
			else{
				right = mid - 1;
			}
			free_bignum(&product);
		}
		bignum w;
		init_bignum_int(&w, ans);
		bignum newdest;
		add(&newdest, dest, &w);
		swap(dest, &newdest);
		free_bignum(&newdest);
		bignum sum;
		add(&sum, &x, &w);
		bignum product;
		mul(&product, &sum, &w);
		free_bignum(&sum);
		free_bignum(&w);
		bignum newrem;
		sub(&newrem, &rem, &product);
		free_bignum(&product);
		swap(&rem, &newrem);
		free_bignum(&newrem);
		free_bignum(&x);
	}
	free_bignum(&rem);
	free_bignum(&two);
	cleanzeros(src);
	cleanzeros(dest);
	for(i = 0; i < 6; i++){
		divby1e9(src);
	}
	for(i = 0; i < 3; i++){
		divby1e9(dest);
	}
	cleanzeros(src);
	cleanzeros(dest);
	trim_self_20(dest);
}

void intdiv2(bignum* dest, bignum* src){
	vector buf;
	init_vector(&buf);
	resize(&buf, (src->a).size);
	int i = 0, carry = 0;
	for(i = (src->a).size - 1; i >= 0; i--){
		set(&buf, i, (carry*base + get(&(src->a), i))/2);
		carry = (carry*base + get(&(src->a), i))&1;
	}
	while(size(&buf) > 1 && back(&buf) == 0){
		pop_back(&buf);
	}
	init_bignum_data(dest, &buf, src->sign, src->e);
	freevector(&buf);
}

void floatdiv2(bignum* dest, bignum* src){
	divby1e9(src);
	bignum fh;
	init_bignum_int(&fh, base >> 1);
	bignum temp2;
	mul(&temp2, src, &fh);
	free_bignum(&fh);
	set_bignum(dest, &temp2);
	free_bignum(&temp2);
}

void mulby1e9(bignum* num){
	if(iszero(num)) return;
	if(num->e > 0) --(num->e);
	else{
		vector vec;
		init_vector(&vec);
		resize(&vec, 1);
		int i, asize = (num->a).size;
		for(i = 0; i < asize; i++){
			push_back(&vec, (num->a).data[i]);
		}
		freevector(&(num->a));
		init_vector(&(num->a));
		copyvector(&(num->a), &vec);
		freevector(&vec);
	}
}

void divby1e9(bignum* num){
	if(iszero(num)) return;
	if((num->a).data[0] == 0) erase(&(num->a), 0);
	else ++(num->e);
}

int mod2(bignum* num){
	/*
	 * returns num % 2
	 */
	 return get(&(num->a), 0)&1;
}

void trimto72dec(bignum* num){
	int removenum = max(0, num->e - 8);
	if(removenum >= (num->a).size){
		free_bignum(num);
		init_bignum_int(num, 0);
	}
	else if(removenum > 0){
		erase_range(&(num->a), 0, removenum);
		num->e = 8;
	}
}

void trimto27dec(bignum* num){
	int removenum = max(0, num->e - 3);
	if(removenum >= (num->a).size){
		free_bignum(num);
		init_bignum_int(num, 0);
	}
	else if(removenum > 0){
		erase_range(&(num->a), 0, removenum);
		num->e = 3;
	}
}

void print_bignum(bignum* num){
	bignum b;
	set_bignum(&b, num);
	trim_self_20(&b);
	print_bignum_debug(&b);
	free_bignum(&b);
}

void trim_self_20(bignum* num){
	if(num->e > 3) trimto27dec(num);
	if(num->e == 3){
		((num->a).data)[0] = ((num->a).data[0]/10000000)*10000000;
	}
	cleanzeros(num);
}

