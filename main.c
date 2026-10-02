#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
typedef long u64;

u64 alloctest(u64 chuncksize){
	struct timespec befaure;
	clock_gettime(0,&befaure);
	void * mem = malloc(chuncksize);
	struct timespec after;
	clock_gettime(0,&after);

	printf("Alocating %d MB @ %x\n",chuncksize/(1024*1024),mem);
	free(mem);
	fflush(stdout);
	//usleep(100000);
	return after.tv_nsec - befaure.tv_nsec;
}


u64 memsettest(u64 chuncksize){
	printf("memset %d MB\n",chuncksize/(1024*1024));
	void * mem = malloc(chuncksize);
	fflush(stdout);
	struct timespec befaure;
	clock_gettime(0,&befaure);


	//memset(mem,0,(size_t)chuncksize>0x1000?0x1000:0);
	char c = rand()%255;
	memset(mem,c,(size_t)chuncksize);
	struct timespec after;
	clock_gettime(0,&after);
	for(int ii = 0 ; ii < 10 ; ii++){
	for(int i = 0 ; i < chuncksize ; i++ )
	{
		assert( ((char*)mem)[i]==c);
	}
	}
	free(mem);
	return after.tv_nsec - befaure.tv_nsec;
}

#define TEST(func,size) alloctestsize = size;\
	allocres =  func(alloctestsize);\
	printf(#func" der %d µs (%ld MB/s)\n",(allocres * 1000000 )/1000000000,(u64)((float)alloctestsize/(float)((float)allocres/(float)1000000000))/(1024*1024));fflush(stdout);




static void* theadtest(void* arg){
	int threadNum = (int)arg;
	for (; ; ){

		u64 allocres=0;
		u64 alloctestsize=0;
		printf("thread %d \n",threadNum);
		/*
		void* ll = malloc(0x100000);
		printf("thread %d : %lx\n",threadNum,ll);
		//usleep(100000);
		unsigned long cbrk = (unsigned long)sbrk(0);
		//printf("thread %d : %lx is free (and brk is 0x%lx)",threadNum,ll,cbrk);
		int dots = rand()%20 +1;
		for(int i = 0 ; i < dots ; i++ )
		{
			printf(".");
		}
		usleep((rand()) % 100);
		//printf("\n");
		free(ll);//*/
		///*
		TEST(alloctest,0x1000*1024);
		//TEST(alloctest, 0x1000*1024*10);
		TEST(memsettest,0x1000*1024);

		//usleep(100000);//*/
	}
	return NULL;

}

int main(){
	u64 allocres=0;
	u64 alloctestsize=0;
	//malloc(0x1000);
	//TEST(alloctest,0x1000*1024*10);
	//TEST(memsettest,0x1000*1024*10);
	const int threadNum = 5;
	pthread_t thread [threadNum];


	for (int i = 0 ; i < threadNum ; i++){
		pthread_create(&thread[i], NULL, &theadtest, (void*)i);
	}
	for (int i = 0 ; i < threadNum ; i++){
		pthread_join(thread[i], NULL);
	}

}

