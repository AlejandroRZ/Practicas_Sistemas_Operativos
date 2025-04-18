#include "devices/timer.h"
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdarg.h>
#include "devices/pit.h"
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "threads/thread.h"
  
/* See [8254] for hardware details of the 8254 timer chip. */

#if TIMER_FREQ < 19
#error 8254 timer requires TIMER_FREQ >= 19
#endif
#if TIMER_FREQ > 1000
#error TIMER_FREQ <= 1000 recommended
#endif

/*List defined to store those threads in sleeping state*/
struct list sleeping_threads;


/* Number of timer ticks since OS booted. */
static int64_t ticks;

/* Number of loops per timer tick.
   Initialized by timer_calibrate(). */
static unsigned loops_per_tick;

// List of all processes who has to wait 
static struct list timer_wait_list;

static intr_handler_func timer_interrupt;
static bool too_many_loops (unsigned loops);
static void busy_wait (int64_t loops);
static void real_time_sleep (int64_t num, int32_t denom);
static void real_time_delay (int64_t num, int32_t denom);

/* Sets up the timer to interrupt TIMER_FREQ times per second,
   and registers the corresponding interrupt. */
void
timer_init (void) 
{
  /**********/
  list_init(&sleeping_threads);
  /************/

  pit_configure_channel (0, 2, TIMER_FREQ);
  intr_register_ext (0x20, timer_interrupt, "8254 Timer");

  // Init of timer_wait_list 
  //list_init(&timer_wait_list);
}

/* Calibrates loops_per_tick, used to implement brief delays. */
void
timer_calibrate (void) 
{
  unsigned high_bit, test_bit;

  ASSERT (intr_get_level () == INTR_ON);
  printf ("Calibrating timer...  ");

  /* Approximate loops_per_tick as the largest power-of-two
     still less than one timer tick. */
  loops_per_tick = 1u << 10;
  while (!too_many_loops (loops_per_tick << 1)) 
    {
      loops_per_tick <<= 1;
      ASSERT (loops_per_tick != 0);
    }

  /* Refine the next 8 bits of loops_per_tick. */
  high_bit = loops_per_tick;
  for (test_bit = high_bit >> 1; test_bit != high_bit >> 10; test_bit >>= 1)
    if (!too_many_loops (loops_per_tick | test_bit))
      loops_per_tick |= test_bit;

  printf ("%'"PRIu64" loops/s.\n", (uint64_t) loops_per_tick * TIMER_FREQ);
}

/* Returns the number of timer ticks since the OS booted. */
int64_t
timer_ticks (void) 
{
  enum intr_level old_level = intr_disable ();
  int64_t t = ticks;
  intr_set_level (old_level);
  return t;
}

/* Returns the number of timer ticks elapsed since THEN, which
   should be a value once returned by timer_ticks(). */
int64_t
timer_elapsed (int64_t then) 
{
  return timer_ticks () - then;
}

/* Sleeps for approximately TICKS timer ticks.  Interrupts must
   be turned on. */
void
timer_sleep (int64_t ticks) 
{
  int64_t start = timer_ticks ();

  ASSERT (intr_get_level () == INTR_ON);

  //Call of timer_wait with the start time plus the number of ticks passed as argument
  //timer_wait(start + ticks);

  /*************************/
  
  if(ticks>0){  
    enum intr_level old_level;
    old_level=intr_disable();   //disable interrupts to prevent racing conditions
    struct thread *cur=thread_current();
    cur->sleepingtime= (ticks + timer_ticks());   //sleep for "ticks" ticks   
    /*insert this thread in sleeping threads list and sort according to its sleeptime (using sleeptime_comparator)*/
    list_insert_ordered (&sleeping_threads,&cur->elem,sleeptime_comparator,NULL);
    thread_block();       //block this thread
    intr_set_level(old_level);  //restore interrupt level
  }

  /************************/
  
}

/* Sleeps for approximately MS milliseconds.  Interrupts must be
   turned on. */
void
timer_msleep (int64_t ms) 
{
  real_time_sleep (ms, 1000);
}

/* Sleeps for approximately US microseconds.  Interrupts must be
   turned on. */
void
timer_usleep (int64_t us) 
{
  real_time_sleep (us, 1000 * 1000);
}

/* Sleeps for approximately NS nanoseconds.  Interrupts must be
   turned on. */
void
timer_nsleep (int64_t ns) 
{
  real_time_sleep (ns, 1000 * 1000 * 1000);
}

/* Busy-waits for approximately MS milliseconds.  Interrupts need
   not be turned on.

   Busy waiting wastes CPU cycles, and busy waiting with
   interrupts off for the interval between timer ticks or longer
   will cause timer ticks to be lost.  Thus, use timer_msleep()
   instead if interrupts are enabled. */

   /* A function which is part of the process scheduling and timer queue management.
      It orders the elements in a list based on the value of time_wakeup.
   */
  /*
  static bool tick_less (const struct list_elem *aa, const struct list_elem *bb, void *aux) {
    ASSERT (aa != NULL); //Verify that the pointers are not null
    ASSERT (bb != NULL);
    struct thread *a = list_entry (aa, struct thread, elem); //Threads that we want to compare.
    struct thread *b = list_entry (bb, struct thread, elem);
    return a->time_wakeup < b->time_wakeup;
  } 

// Timer Wait 
void timer_wait (int64_t ticks)
{
  // Sleep for 0 or less ticks
  if (ticks <= 0)
    return;

  struct thread *cur = thread_current (); //Pointer to the current waiting thread
  enum intr_level old_level = intr_disable (); //Disable temporarily the interruptions
  cur->time_wakeup = ticks; //Current thread's wake up time actualization

  list_insert_ordered(&timer_wait_list, &cur->elem, tick_less, NULL); //We insert the current thread into the list
  thread_block(); //Block the current thread                          //of threads actively waiting
  intr_set_level (old_level); //The interrupt level is restored to its previous value
}

// Timer Wakeup function that allows threads which have been waiting
// certain amount of time in the list timer_wait_list.
void timer_wakeup()
{
  struct thread *t; //Pointer for a thread struct
  struct list_elem *cur = list_begin (&timer_wait_list), *next; //Inicialize pointer curr to the head of timer_wait_list
                                                                //declares pointer next
  if (list_empty (&timer_wait_list))
    return;

  while (cur != list_end (&timer_wait_list))
    {
      next = list_next (cur);
      t = list_entry (cur, struct thread, elem);
      if (t->time_wakeup > timer_ticks())
        break;

      // Remove the thread from timer_wait_list
      // then unblock it
      enum intr_level old_level; 
      old_level = intr_disable ();
      list_remove (cur);
      thread_unblock (t);
      intr_set_level (old_level);
      cur = next;
    }
}
*/

void
timer_mdelay (int64_t ms) 
{
  real_time_delay (ms, 1000);
}

/* Sleeps for approximately US microseconds.  Interrupts need not
   be turned on.

   Busy waiting wastes CPU cycles, and busy waiting with
   interrupts off for the interval between timer ticks or longer
   will cause timer ticks to be lost.  Thus, use timer_usleep()
   instead if interrupts are enabled. */
void
timer_udelay (int64_t us) 
{
  real_time_delay (us, 1000 * 1000);
}

/* Sleeps execution for approximately NS nanoseconds.  Interrupts
   need not be turned on.

   Busy waiting wastes CPU cycles, and busy waiting with
   interrupts off for the interval between timer ticks or longer
   will cause timer ticks to be lost.  Thus, use timer_nsleep()
   instead if interrupts are enabled.*/
void
timer_ndelay (int64_t ns) 
{
  real_time_delay (ns, 1000 * 1000 * 1000);
}

/* Prints timer statistics. */
void
timer_print_stats (void) 
{
  printf ("Timer: %"PRId64" ticks\n", timer_ticks ());
}

/* Timer interrupt handler. */
static void
timer_interrupt (struct intr_frame *args UNUSED)
{
  ticks++;
  thread_tick ();
  //timer_wakeup();
  /**************************/
  struct list_elem *front;
  struct thread *entry;
  while(true){
    if(list_empty(&sleeping_threads)==true)
      break;
    front=list_front(&sleeping_threads);
    entry=list_entry(front,struct thread,elem);
    if(entry->sleepingtime>ticks)
      break;
    else{
      list_remove(front);
      thread_unblock(entry);
    }
  }
  /*************************/
}

/* Returns true if LOOPS iterations waits for more than one timer
   tick, otherwise false. */
static bool
too_many_loops (unsigned loops) 
{
  /* Wait for a timer tick. */
  int64_t start = ticks;
  while (ticks == start)
    barrier ();

  /* Run LOOPS loops. */
  start = ticks;
  busy_wait (loops);

  /* If the tick count changed, we iterated too long. */
  barrier ();
  return start != ticks;
}

/* Iterates through a simple loop LOOPS times, for implementing
   brief delays.

   Marked NO_INLINE because code alignment can significantly
   affect timings, so that if this function was inlined
   differently in different places the results would be difficult
   to predict. */
static void NO_INLINE
busy_wait (int64_t loops) 
{
  while (loops-- > 0)
    barrier ();
}

/* Sleep for approximately NUM/DENOM seconds. */
static void
real_time_sleep (int64_t num, int32_t denom) 
{
  /* Convert NUM/DENOM seconds into timer ticks, rounding down.
          
        (NUM / DENOM) s          
     ---------------------- = NUM * TIMER_FREQ / DENOM ticks. 
     1 s / TIMER_FREQ ticks
  */
  int64_t ticks = num * TIMER_FREQ / denom;

  ASSERT (intr_get_level () == INTR_ON);
  if (ticks > 0)
    {
      /* We're waiting for at least one full timer tick.  Use
         timer_sleep() because it will yield the CPU to other
         processes. */                
      timer_sleep (ticks); 
    }
  else 
    {
      /* Otherwise, use a busy-wait loop for more accurate
         sub-tick timing. */
      real_time_delay (num, denom); 
    }
}

/* Busy-wait for approximately NUM/DENOM seconds. */
static void
real_time_delay (int64_t num, int32_t denom)
{
  /* Scale the numerator and denominator down by 1000 to avoid
     the possibility of overflow. */
  ASSERT (denom % 1000 == 0);
  busy_wait (loops_per_tick * num / 1000 * TIMER_FREQ / (denom / 1000)); 
}

