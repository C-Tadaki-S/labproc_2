#include "sched.h"
#include "./libmem/mem.h"

extern void enable_irq(int);
extern ttb_l1_t kernel_ttb; /* declarado em sched.c */

void system_main(void)
{
  mmu_stop();
  mmu_flat(&kernel_ttb);
  mmu_start(&kernel_ttb); /* MMU ligada */
  sched_init();
  asm volatile("b task_switch");
}

int a = 23;
int b = 42;

/*
 * Ponto de entrada do primeiro task.
 */
void user1_main(void)
{
  int i;
  a = 100;
  b = 20;
  for (;;)
  {
    for (i = 0; i < a; i++)
    {
      asm volatile("nop");
    }
    asm volatile("nop");
    yield();
  }
}

/*
 * Ponto de entrada do segundo task.
 */
void user2_main(void)
{
  int i;
  a = 200;
  for (;;)
  {
    for (i = 0; i < a; i++)
    {
      asm volatile("nop");
    }
    asm volatile("nop");
    yield();
  }
}

void user3_main(void)
{
  int i;
  a = 300;
  for (;;)
  {
    for (i = 0; i < a; i++)
    {
      asm volatile("nop");
    }
    asm volatile("nop");
    yield();
  }
}
