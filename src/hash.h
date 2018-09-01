#ifndef HASH_H
#define HASH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef void (*dtor_t)(void *);
typedef struct bucket_s { // Linked list
	unsigned long key;
	void *val;
	struct bucket_s *cdr; // Next node
} bucket_t;
typedef struct {
	int size; // Number of bucket locations
	int members;
	int rehash;
	bucket_t **pool;
	dtor_t destructor;
} table_t;

extern unsigned long (*hash_function)(char *);

unsigned long fnv_1a(char *);
unsigned long nocase_fnv_1a(char *);
table_t *new_table(int,dtor_t);
void free_table(table_t *);
void insert(table_t *,char *,void *);
void *lookup(table_t *,char *);
void expunge(table_t *,char *);
void rehash(table_t *,int);
#endif
