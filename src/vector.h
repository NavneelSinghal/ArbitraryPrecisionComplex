#ifndef VECTOR_H
#define VECTOR_H

typedef struct vectr{
    int size;
    int *data;
    int capacity;
} vector;

void init_vector (vector*);
int size (vector*);
void resize (vector*, int);
void freevector (vector*);
void erase (vector*, int);
void erase_range (vector*, int, int);
void push_back (vector*, int);
void pop_back (vector*);
int get (vector*, int);
void set (vector*, int, int);
void copyvector (vector*, vector*);
int back (vector*);
void print_vector (vector*);
void reverse (vector* v);

#endif
