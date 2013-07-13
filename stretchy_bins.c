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
  uint32_t capacity;
  uint32_t fill;
} stretchy_bin_t;

void init_stretchy_bin(stretchy_bin_t *bin, uint32_t capacity){
  check_calloc(uint32_t,bin->elts,capacity,"bin->elts");
  bin->capacity = capacity;
  bin->fill = 0;
}

stretchy_bin_t *new_stretchy_bin(uint32_t capacity){
  stretchy_bin_t *retbin;
  check_malloc(stretchy_bin_t,retbin,1,"new stretchy bin");
  init_stretchy_bin(retbin,capacity);
  return retbin;
}

void find_in_stretchy_bin(int *ret,stretchy_bin_t *bin, uint32_t value){
  int i,j;
  j=0;
  for(i=0;i<bin->fill;i++){
    if(bin->elts[i]==value){ // if we find this value in the bin
	j=1;
	break;
    }
  }
  ret[0] = i;
  ret[1] = j;
}

void add_to_stretchy_bin(stretchy_bin_t *bin, uint32_t item){
  bin->elts[bin->fill++] = item;
  if (bin->fill==bin->capacity){
    bin->capacity *= 2;
    check_realloc(uint32_t,bin->elts,bin->capacity,"bin->elts");
  }
}

void print_stretchy_bin(stretchy_bin_t *bin,char *desc){
  int i;
  printf("Bin (%s) has capacity %d and %d items.\n",desc,bin->capacity,bin->fill);
  printf("In bin:");
  for (i=0;i<bin->fill;i++){
    printf(" %u",bin->elts[i]);
  }
  printf("\n");
}

void print_stretchy_bin_of_bins(stretchy_bin_t *bin,char *desc){
  int i;
  char newdesc[MAX_LINE_LEN];
  printf("Bin (%s) has capacity %d and %d items.\n",desc,bin->capacity,bin->fill);
  for (i=0;i<bin->fill;i++){
    //printf(" %u",bin->elts[i]);
    sprintf(newdesc,"%s (part %d)",desc,i);
    print_stretchy_bin((stretchy_bin_t *)bin->elts[i],newdesc);
  }
}

void free_stretchy_bin(stretchy_bin_t *bin){
  //printf("In theory, going to free bin at address %u\n",(uint32_t)bin);
  free(bin->elts);
}

void free_stretchy_bin_of_bins(stretchy_bin_t *bin){
  int i;
  //printf("OK, going to try to free bin of bins at address %u\n",(uint32_t)bin);
  for(i=0;i<bin->fill;i++){
    //printf("Allegedly freeing part %d of %d (address %u)\n",i,bin->fill,bin->elts[i]);
    free_stretchy_bin((stretchy_bin_t *)(bin->elts[i]));
    free((stretchy_bin_t *)bin->elts[i]);
  }
  free(bin->elts);
}

void test_stretchy_bin(){
  stretchy_bin_t testbin;
  uint32_t i;
  
  init_stretchy_bin(&testbin,1);

  for (i=0;i<10;i++){
    add_to_stretchy_bin(&testbin,i);
  }

  print_stretchy_bin(&testbin,"testbin");

  free_stretchy_bin(&testbin);
}

