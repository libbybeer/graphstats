#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_LINE_LEN 2048

#define check_malloc(type,ptr,num,desc)			\
  if (( (ptr) = (type *)malloc(num*sizeof(type))) == NULL){	\
    fprintf( stderr, "Can't malloc memory for %s.\n",		\
	     desc);						\
    exit(1);							\
    }

#define check_calloc(type,ptr,num,desc)			\
  if (( (ptr) = (type *)calloc(num,sizeof(type))) == NULL){	\
    fprintf( stderr, "Can't calloc memory for %s.\n",		\
	     desc);						\
    exit(1);							\
    }

#define check_realloc(type,ptr,num,desc)			\
  if (( (ptr) = (type *)realloc((ptr),num*sizeof(type))) == NULL){	\
    fprintf( stderr, "Can't realloc memory for %s.\n",		\
	     desc);						\
    exit(1);							\
    }

typedef struct {
  uint32_t *elts;
  uint32_t *cnts;
  uint32_t capacity;
  uint32_t fill;
} stretchy_hist_t;

void init_stretchy_hist(stretchy_hist_t *hist, uint32_t capacity);

stretchy_hist_t *new_stretchy_hist(uint32_t capacity);

void find_in_stretchy_hist(int *ret,stretchy_hist_t *hist, uint32_t value);

void add_to_stretchy_hist(stretchy_hist_t *hist, uint32_t item);

void print_stretchy_hist(stretchy_hist_t *hist,char *desc);

void print_stretchy_hist_of_hists(stretchy_hist_t *hist,char *desc);

void free_stretchy_hist(stretchy_hist_t *hist);

void free_stretchy_hist_of_hists(stretchy_hist_t *hist);

void test_stretchy_hist();
