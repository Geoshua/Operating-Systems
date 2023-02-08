#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>

int tflag = 0, vflag = 0, tcount = 0;


void *printperf(int low,int high);
static int is_perfect(uint64_t num)
{
	uint64_t i, sum;
	if (num < 2) {
		return 0;
	}
	for (i = 2, sum = 1; i*i <= num; i++) {
		if (num % i == 0) {
			sum += (i*i == num) ? i : i + num / i;
		}
	}
	return (sum == num);
}

int main(int argc, char *argv[])
{
	int opt, thread_count = 1,low = 1,high = 10000,i=0;
	while((opt = getopt(argc,argv,"s:e:t:v"))!= -1)
	{
		switch(opt)
		{
			case 's':
				low = atoi(optarg);
				break;
			case 'e':
				high = atoi(optarg);
				break;
			case 't':
				tflag = 1;
				thread_count = atoi(optarg);
				break;
			case'v':
				vflag = 1;
			default:
				break;
		}
	}
	if(tflag){
		pthread_t thread[thread_count];

		int min = low, max = high;
		for(i = thread_count; i > 0 && min <= max ;i--){
			int range = 1 + max - min;
			int sub_range = range/i;
			//printf("hello");
			printf("r %d sr %d i %d\n",min,min+sub_range-1, i);
			if(sub_range == 0) sub_range = 1;
			pthread_create(&thread[i],NULL,printperf(min,min+sub_range-1),NULL);
			min += sub_range;
		}
		//printf("hello");
		for(i = thread_count;i>=0;i--){
			pthread_join(thread[i],NULL);
		}
	}
	else{
		printperf(low,high);
	}
	return 0;
}

void *printperf(int low,int high){
	if(vflag){
		char temp[50], lowc[5],highc[5],tcountc[5];
		sprintf(lowc, "%d", low);
		sprintf(lowc,"%d",low);
		sprintf(highc,"%d",high);
		sprintf(tcountc,"%d",tcount);
		strcpy(temp,"perfect: t");
		strcat(temp,tcountc);
		strcat(temp," searching [");
		strcat(temp,lowc);
		strcat(temp,",");
		strcat(temp,highc);
		strcat(temp,"]\n");
		fprintf(stderr,"%s",temp);
		tcount++;
		sprintf(lowc,"%d",low);
		printf("lowc %s\n",lowc);
	}
	for(int i = low;i<=high;i++)
	{
		//printf("c %d\n",i);
		if(is_perfect(i))
		{
			printf("%d\n",i);
		}
	}

	return (NULL);
}