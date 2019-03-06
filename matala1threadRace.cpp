#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <list>
#include <pthread.h>


int count = 0;
int value = 0;
volatile bool idle_threads = true;


enum lock_type {LOCK_NONE=0, LOCK_OS=1, LOCK_X86=2};
int lock;

pthread_mutex_t os_mutex;
unsigned int x86_mutex;

std::list<pthread_t*> threads;

void parse_args(int argc, char** argv)
{
  if ( argc < 3 ) {
    printf(
      "USAGE: lab0 <threads> <lock>\n"
      "\n"
      "ARGUMENTS\n"
      "  <threads>  Number of threads to start, positive integer\n"
      "  <lock>     0 = Don't use any lock\n"
      "             1 = Use OS-specific lock\n"
      "             2 = Use x86 assembly lock\n"
      "\n"
      "Try running `lab0 10 0' first, then increase to, e.g. `lab0 1000 0'\n"
      "and run it many times to see various protection misses.  Then try\n"
      "running with a lock, e.g. `lab0 1000 1` or `lab0 1000 2`.\n"
      "\n");
    exit(1);
  }

  if ( !(sscanf(argv[1], "%d", &count) == 1 && count > 0) ) {
    printf("<threads> must be a positive integer: %s\n\n", argv[1]);
    exit(1);
  }

  if ( !(sscanf(argv[2], "%d", reinterpret_cast<int*>(&lock)) == 1 && (lock>=0 && lock<=2)) ) {
    printf("<lock> must be either of 0, 1, 2 or 3: %s\n\n", argv[2]);
    exit(1);
  }
}


int try_set_lock(unsigned int *addr)
{
  
  register int content = 1;

  asm volatile (
    "xchgl %0, %1" : /* atomic operation; freezes other CPUs */
    "=r" (content),  /* output operand, write-only, any register */
    "=m" (*addr) :   /* output operand, directly in memory */
    "0" (content),   /* input operand, any register, matching digit %0 */
    "m" (*addr));    /* input operand, directly from memory */

  return content;
}


void* worker(void*)
{

  while ( idle_threads )
    usleep(100000);

  if ( lock == LOCK_OS )
    pthread_mutex_lock(&os_mutex);

  else if ( lock == LOCK_X86 )
    while ( try_set_lock(&x86_mutex) != 0 )
      ; // loop

  
  value = value + 1;

  
  if ( lock == LOCK_OS )
    pthread_mutex_unlock(&os_mutex);

  else if ( lock == LOCK_X86 )
    x86_mutex = 0;

  return NULL;
}

int main(int argc, char** argv)
{
  parse_args(argc, argv);

  printf("DAT320 Operating Systems at University of Stavanger\n"
         "Lab 0, by Christian Stigen Larsen\n\n");

  printf("Starting threads w/locking scheme: %s\n",
    lock==LOCK_NONE? "No lock" :
    lock==LOCK_OS?   "pthread_mutex_lock" :
    lock==LOCK_X86?  "x86 xchg instruction" :
                     "x86 lock btr instruction");

  pthread_mutex_init(&os_mutex, NULL);

  for ( int n=0, i; n < count; ++n ) {
    printf("%d / %d\r", n, count); fflush(stdout);

    threads.push_back(new pthread_t());

    if ( (i = pthread_create(threads.back(), NULL, &worker, NULL)) != 0 ) {
      printf("Error: Cannot create thread %d w/error %d\n", n, i);
      exit(1);
    }
  }

  
  idle_threads = false;

  printf("Joining threads\n");

  for ( int n=0, i; !threads.empty(); ++n ) {
    printf("%d / %d %-40s\r", n, count, " "); fflush(stdout);

    if ( (i = pthread_join(*threads.back(), NULL)) != 0 ) {
      printf("Error: pthread_join returned %d\n", n);
      exit(1);
    }

    threads.pop_back();
  }

  printf("Protection misses: %d %-40s\n", count - value, " ");

  if ( value == count ) {
    printf("OK\n");
    return 0;
  }

  printf("FAIL\n");
  return 1;
}
