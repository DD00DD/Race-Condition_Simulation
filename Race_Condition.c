/*
 3 users use the DB (for each, a thread is spawned).
 Thread i does queryNo[i] queries for its user.
 A global "total_queries" is maintained. Each thread
 updates it after each of its queries.

 This has a race condition on variable total_queries. 

 Note that if you have maxed out your allotted clones, 
   one (or more) child may never execute its function, 
   although it exists. So parent can't terminate 
   because it waits forever for child (do ps -l or 
   ps -lu USERID and kill the offending child)
*/

#define _GNU_SOURCE  //for Ubuntu
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>

int queryNo[3] = {1011, 1012, 1013};

int total_queries = 0;

void *userThread(void* num) 
{
   int i;
   int q = (int)(long) num;

   for (i = 0; i < queryNo[q]; i++) { //handle queries
       total_queries = total_queries + 1;
   }

   return NULL;
}

int main(int argc, char *argv[]) 
{
   pthread_t t1;
   pthread_t t2;
   pthread_t t3;

   pthread_create(&t1, NULL, userThread, (void*) (long) 0);   
   pthread_create(&t2, NULL, userThread, (void*) (long) 1);
   pthread_create(&t3, NULL, userThread, (void*) (long) 2);

   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
   pthread_join(t3, NULL);

   printf("%d total_queries should be 3036 and it is %d\n", getpid(), total_queries);
   return 0;
}
      
