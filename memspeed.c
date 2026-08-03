#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
typedef long u64;

u64 alloctest(u64 chuncksize){
	printf("Alocating %d MB\n",chuncksize/(1024*1024));
	u64 start = clock();
	void * mem = malloc(chuncksize);
	free(mem);
	return clock() - start;
}


u64 memsettest(u64 chuncksize){
	printf("memset %d MB\n",chuncksize/(1024*1024));
	void * mem = malloc(chuncksize);
	
	u64 start = clock();
	
	memset(mem,0,(size_t)chuncksize);
	
	u64 ret = clock() -start;
	free(mem);
	return ret;
}

#define TEST(func,size) alloctestsize = size;\
	allocres =  func(alloctestsize);\
	printf(#func" der %d µs (%ld MB/s)\n",(allocres * 1000000 )/CLOCKS_PER_SEC,(u64)((float)alloctestsize/(float)((float)allocres/(float)CLOCKS_PER_SEC))/(1024*1024));





int main(){
	u64 allocres=0;
	u64 alloctestsize=0;
	TEST(alloctest,0x1000*1024*10);
	TEST(memsettest,0x1000*1024*10);
}
