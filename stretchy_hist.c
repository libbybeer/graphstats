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

void init_stretchy_hist(stretchy_hist_t *hist, uint32_t capacity){
  check_calloc(uint32_t,hist->elts,capacity,"hist->elts");
  check_calloc(uint32_t,hist->cnts,capacity,"hist->cnts");
  hist->capacity = capacity;
  hist->fill = 0;
}

stretchy_hist_t *new_stretchy_hist(uint32_t capacity){
  stretchy_hist_t *rethist;
  check_malloc(stretchy_hist_t,rethist,1,"new stretchy hist");
  init_stretchy_hist(rethist,capacity);
  return rethist;
}

void find_in_stretchy_hist(int *ret,stretchy_hist_t *hist, uint32_t value){
  int i,j;
  j=0;
  for(i=0;i<hist->fill;i++){
    if(hist->elts[i]==value){ // if we find this value in the hist
	j=1;
	break;
    }
  }
  ret[0] = i;
  ret[1] = j;
}

void add_to_stretchy_hist(stretchy_hist_t *hist, uint32_t item){
  hist->cnts[hist->fill] = 1;
  hist->elts[hist->fill++] = item;
  if (hist->fill==hist->capacity){
    hist->capacity *= 2;
    check_realloc(uint32_t,hist->elts,hist->capacity,"hist->elts");
    check_realloc(uint32_t,hist->cnts,hist->capacity,"hist->cnts");
  }
}

void print_stretchy_hist(stretchy_hist_t *hist,char *desc){
  int i;
  printf("Hist (%s) has capacity %d and %d items.\n",desc,hist->capacity,hist->fill);
  printf("In hist:");
  for (i=0;i<hist->fill;i++){
    printf(" %u",hist->elts[i]);
    printf("(%u)",hist->cnts[i]);
  }
  printf("\n");
}

void print_stretchy_hist_of_hists(stretchy_hist_t *hist,char *desc){
  int i;
  char newdesc[MAX_LINE_LEN];
  printf("Hist (%s) has capacity %d and %d items.\n",desc,hist->capacity,hist->fill);
  for (i=0;i<hist->fill;i++){
    //printf(" %u",hist->elts[i]);
    sprintf(newdesc,"%s (part %d)",desc,i);
    print_stretchy_hist((stretchy_hist_t *)hist->elts[i],newdesc);
  }
}

void free_stretchy_hist(stretchy_hist_t *hist){
  //printf("In theory, going to free hist at address %u\n",(uint32_t)hist);
  free(hist->elts);
  free(hist->cnts);
}

void free_stretchy_hist_of_hists(stretchy_hist_t *hist){
  int i;
  //printf("OK, going to try to free hist of hists at address %u\n",(uint32_t)hist);
  for(i=0;i<hist->fill;i++){
    //printf("Allegedly freeing part %d of %d (address %u)\n",i,hist->fill,hist->elts[i]);
    free_stretchy_hist((stretchy_hist_t *)(hist->elts[i]));
    free((stretchy_hist_t *)hist->elts[i]);
  }
  free(hist->elts);
  free(hist->cnts);
}

void test_stretchy_hist(){
  stretchy_hist_t testhist;
  uint32_t i;
  
  init_stretchy_hist(&testhist,1);

  for (i=0;i<10;i++){
    add_to_stretchy_hist(&testhist,i);
  }

  print_stretchy_hist(&testhist,"testhist");

  free_stretchy_hist(&testhist);
}

