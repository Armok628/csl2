#include "hash.h"
unsigned long (*hash_function)(char *)=&fnv_1a;
const unsigned long fnv_prime=0x100000001b3;
const unsigned long fnv_offset=0xcbf29ce484222325;
unsigned long fnv_1a(char *str)
{
	unsigned long key=fnv_offset;
	char c;
	for (c=*str++;c;c=*str++) {
		key^=c;
		key*=fnv_prime;
	}
	return key;
}
unsigned long nocase_fnv_1a(char *str)
{
	unsigned long key=fnv_offset;
	char c;
	for (c=*str++;c;c=*str++) {
		if ('a'<=c&&c<='z')
			c+='A'-'a';
		key^=c;
		key*=fnv_prime;
	}
	return key;
}
bucket_t *new_bucket(unsigned long key,void *val)
{
	bucket_t *b=malloc(sizeof(bucket_t));
	b->key=key;
	b->val=val;
	return b;
}
void free_location(bucket_t *b,dtor_t d)
{
	bucket_t *t;

	while (b) {
		t=b->cdr;
		if (d)
			d(b->val);
		free(b);
		b=t;
	}
}
table_t *new_table(int size,dtor_t d)
{ /* Negative size enables automatic resizing */
	table_t *table;
	if (!size)
		return NULL;
	table=calloc(1,sizeof(table_t));
	if (size<=0) {
		size=-size;
		table->rehash=1;
	}
	table->size=size;
	table->pool=calloc(size,sizeof(bucket_t *));
	table->destructor=d;
	return table;
}
void free_table(table_t *table)
{
	int i;
	for (i=0;i<table->size;i++) /* Check all locations */
		if (table->pool[i])
			free_location(table->pool[i],table->destructor);
	free(table->pool);
	free(table);
}
void insert_bucket(table_t *table,bucket_t *entry)
{ /* Never triggers rehashing */
	unsigned long key=entry->key;
	bucket_t **loc=&table->pool[key%table->size]; /* : Bucket location in pool */
	if (*loc) { /* Bucket(s) already in location */
		bucket_t *b=*loc;
		while (b->cdr&&b->key!=key) /* Look for identical bucket or end */
			b=b->cdr;
		if (b->key==key) { /* Found identical bucket */
			if (table->destructor)
				table->destructor(b->val);
			b->val=entry->val;
			free(entry);
			return;
		} else /* Found end */
			b->cdr=entry;
	} else /* No buckets at location */
		*loc=entry;
	entry->cdr=NULL;
	table->members++;
}
void insert(table_t *table,char *str,void *val)
{ /* May trigger rehashing */
	insert_bucket(table,new_bucket(hash_function(str),val));
	if (table->rehash&&table->members>table->size)
		rehash(table,table->size*2);
}
void *lookup(table_t *table,char *str)
{
	unsigned long key=hash_function(str);
	bucket_t *b=table->pool[key%table->size]; /* : Bucket in pool */
	while (b&&b->key!=key) /* Look for identical bucket or end */
		b=b->cdr;
	return b?b->val:NULL;
}
void expunge(table_t *table,char *str)
{
	unsigned long key=hash_function(str);
	bucket_t **loc=&table->pool[key%table->size];
	bucket_t *p=NULL,*b=*loc;
	while (b&&b->key!=key) { /* Look for entry */
		p=b; /* Remember previous node */
		b=b->cdr;
	}
	if (!b) /* Entry not found */
		return;
	if (p) /* Previous node */
		p->cdr=b->cdr;
	else
		*loc=b->cdr;
	if (table->destructor)
		table->destructor(b->val);
	free(b);
}
void rehash(table_t *table,int newsize)
{
	bucket_t **oldpool=table->pool;
	int i,oldsize=table->size;
	/*fprintf(stderr,"Resizing table to %d\n",newsize);*/
	table->pool=calloc(newsize,sizeof(bucket_t *));
	table->size=newsize;
	table->members=0;
	for (i=0;i<oldsize;i++) {
		bucket_t *l=oldpool[i];
		while (l) {
			bucket_t *t=l;
			l=l->cdr;
			insert_bucket(table,t);
		}
	}
	free(oldpool);
}
