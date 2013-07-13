#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stretchy_bins.h"


void process_file(){

  // first, read in each line of the file, and
  // put the first element of each line into
  // a stretchy bin (if it's not in the bin already).

  FILE *infile;
  char line[MAX_LINE_LEN];
  int a,b,c,i,j,k;
  int bin_index;
  stretchy_bin_t abin;
  stretchy_bin_t bbin;
  stretchy_bin_t cbin;
  stretchy_bin_t *bptr;
  stretchy_bin_t *cptr;
  stretchy_bin_t *bptr2;
  stretchy_bin_t *cptr2;
  int found_index[2];

  init_stretchy_bin(&abin,1);
  init_stretchy_bin(&bbin,1);
  init_stretchy_bin(&cbin,1);

  // ought to un-hard-code the filename
  // ought to make the sscanf into something robust
  infile = fopen("mystruct_test_list.txt","rt");
  while(fgets(line,MAX_LINE_LEN,infile) != NULL){
    sscanf(line, "%d %d %d",&a,&b,&c);
    //printf("A: %d; B: %d; C: %d\n",a,b,c);
    j=0;
    for(i=0;i<abin.fill;i++){
      if(abin.elts[i]==a){ // if we find this A in the bin
	j=1;
	bin_index=i;
	break;
      }
    }
    if(j==0){ // if we don't find this A in the bin
      bin_index = abin.fill;
      add_to_stretchy_bin(&abin,(uint32_t)a);
      add_to_stretchy_bin(&bbin,(uint32_t)(new_stretchy_bin(1)));
      add_to_stretchy_bin(&cbin,(uint32_t)(new_stretchy_bin(1)));
    }
    j=0;
    bptr = (stretchy_bin_t *)bbin.elts[bin_index];
    cptr = (stretchy_bin_t *)cbin.elts[bin_index];
    for (i=0;i<bptr->fill;i++){
      if ((bptr->elts[i]==b) && (cptr->elts[i]==c)){
	j=1;
	break;
      }
    }
    if(j==0){
      add_to_stretchy_bin((stretchy_bin_t *)(bbin.elts[bin_index]),b);
      add_to_stretchy_bin((stretchy_bin_t *)(cbin.elts[bin_index]),c);
    }
  } // ends "while(fgets(line,MAX_LINE_LEN,infile) != NULL)"
  fclose(infile);

  for(i=0;i<abin.fill;i++){
    bptr = (stretchy_bin_t *)bbin.elts[i];
    cptr = (stretchy_bin_t *)cbin.elts[i];
    for(j=0;j<bptr->fill;j++){
      find_in_stretchy_bin(found_index,&abin,bptr->elts[j]);
      if(found_index[1]==1){
	bptr2 = (stretchy_bin_t *)bbin.elts[found_index[0]];
	cptr2 = (stretchy_bin_t *)bbin.elts[found_index[0]];
	for(k=0;k<bptr2->fill;k++){
	  printf("chain: %u -(%u)-> %u -(%u)-> %u\n",
		 abin.elts[i],cptr->elts[j],bptr->elts[j],cptr2->elts[k],bptr2->elts[k]);
	}
      }
    }
  }
  //print_stretchy_bin(&abin,"abin");
  //print_stretchy_bin_of_bins(&bbin,"bbin");
  //print_stretchy_bin_of_bins(&cbin,"cbin");

  free_stretchy_bin_of_bins(&cbin);
  free_stretchy_bin_of_bins(&bbin);
  free_stretchy_bin(&abin);
}


int main(int argc, char **argv){
  printf("Int is %d bits on this system.\n",8*sizeof(int));
  //test_stretchy_bin();
  process_file();
  return 0;
}
