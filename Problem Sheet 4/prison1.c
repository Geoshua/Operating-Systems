#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h> 

int tr_flag=0; //toggle tracing

int win=1;
int seed=0;
int count_w=0;
int count_l=0;
pthread_mutex_t s_lock; 
pthread_mutex_t m_lock[100];      
int drawer[100]; 
int prisoners[100];

void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        srandom(seed);
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + random() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

static void run_threads(int n, void* (*proc)(void *))
{
    int count_l_w=0;
    int count_l_l=0;
    for (size_t k = 0; k < n;k++)
    {   
        shuffle(drawer,100);
        pthread_t tid[100];
        for (int i = 0; i < 100; i++)
        {   
            if (tr_flag==1)
            {
                perror("creating thread");
            } 
          
		    pthread_create(&tid[i],NULL,proc,&prisoners[i]);
        }
    
        for (int i = 0; i < 100; i++)
        {
            if (tr_flag==1)
            {
                perror("closing thread");
            }
            pthread_join(tid[i],NULL);
        }
        if (win==1)
        {
            count_l_w++;
            
    
        }
        else
        {
        count_l_l++;
        }

    }
    printf("%d - wins.",count_l_w);
       
    printf("%d - lose.",count_l_l);
    count_l=0;
    count_w=0;
    win=1;

}




static double timeit(int n, void* (*proc)(void *))
{
    clock_t t1, t2;
    t1 = clock();
    run_threads(n, proc);
    t2 = clock();
    return ((double) t2 - (double) t1) / CLOCKS_PER_SEC * 1000;
}


static void *random_global(void *num) 
{   
    int attempted[100]={0};
    int count=0;
    int choice;
    pthread_mutex_lock(&s_lock);
    while (count<50)
    {
        choice=(random()%100)+1;
        if (attempted[choice-1]!=1)
        {    
            if (drawer[choice-1]!=*(int*)num)
            {
                attempted[choice-1]=1;
                count++;
            }
            else
            {
                pthread_mutex_unlock(&s_lock);
                count_w++;
                pthread_exit(0);
            }
        }   
    }
    pthread_mutex_unlock(&s_lock);
    win=0;
    count_l++;
    pthread_exit(0);
    return NULL;
}

static void *strategy_global(void *num) 
{   
    int attempted[100]={0};
    int count=0;
    int choice;
    choice=*(int*)num;
    pthread_mutex_lock(&s_lock);
    while (count<50)
    {
        if (attempted[choice-1]!=1)
        {    
            if (drawer[choice-1]!=*(int*)num)
            {
                attempted[choice-1]=1;
                choice=drawer[choice-1];
                count++;
            }
            else
            {
                pthread_mutex_unlock(&s_lock);
                 count_w++;
               pthread_exit(0);
            }
        }   
    }
   // printf("lost %d\n",*(int*)num);
    pthread_mutex_unlock(&s_lock);
    win=0;
    count_l++;
    pthread_exit(0);
    return NULL;
}

static void *strategy_drawer(void *num) 
{   
    int attempted[100]={0};
    int count=0;
    int choice;
    choice=*(int*)num;
    int tmp;
    while (count<50)
    {
        tmp=choice-1;
        pthread_mutex_lock(&m_lock[tmp]);
        if (attempted[choice-1]!=1)
        {    
            if (drawer[choice-1]!=*(int*)num)
            {
                attempted[choice-1]=1;
                choice=drawer[choice-1];
                count++;
            }
            else
            {
                pthread_mutex_unlock(&m_lock[choice-1]);
                 count_w++;
                pthread_exit(0);
            }
        }   
        pthread_mutex_unlock(&m_lock[tmp]);
    }
    
    win=0;
     count_l++;
    pthread_exit(0);
    return NULL;
}


static void *random_drawer(void *num) 
{   
    int attempted[100]={0};
    int count=0;
    int choice;
    while (count<50)
    {
        choice=random()%100+1;
        pthread_mutex_lock(&m_lock[choice-1]);
        if (attempted[choice-1]!=1)
        {    
            if (drawer[choice-1]!=*(int*)num)
            {
                attempted[choice-1]=1;
                count++;
            }
            else
            {
                pthread_mutex_unlock(&m_lock[choice-1]);
                 count_w++;
                pthread_exit(0);
            }
        }   
        pthread_mutex_unlock(&m_lock[choice-1]);
    }
    
    win=0;
     count_l++;
    pthread_exit(0);
    return NULL;
}


int main(int argc, char *argv[]) 
{
    int opt;
    seed=time(0);
    int games=100;
    
  	pthread_attr_t attr; 
    pthread_attr_init(&attr);
    while((opt = getopt(argc, argv, "n:s:")) != -1) 
	{ 
		switch (opt)
		{
			case 'n': 
			{	
				games= atoi(optarg); 
            
			}
			break;
			case 's': 
			{   
              
                seed = atoi(optarg);
		
			}
            break;
            

	
			default:
				break;
		}
    }

 
    srandom(seed);
    if (pthread_mutex_init(&s_lock, NULL) != 0)
    {
            perror("\n mutex init failed\n");
            return 1;
    }
    for (size_t i = 0; i < 100; i++)
    {
        if (pthread_mutex_init(&m_lock[i], NULL) != 0)
        {
            perror("\n mutex init failed\n");
            return 1;
        }
        
        drawer[i]=i+1;  
        prisoners[i]=i+1;  
    }
    shuffle(drawer,100);

    printf("time:%lf  random global\n",timeit(games,random_global));

    printf("time:%lf  random drawer\n",timeit(games,random_drawer));
   
    printf("time:%lf  strategy global\n",timeit(games,strategy_drawer));
   
    printf("time:%lf  strategy global\n",timeit(games,strategy_global));
   

    
    exit(EXIT_SUCCESS);
}


