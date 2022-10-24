#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct snode {
  void * data;
  struct snode * next;
}
snode;

typedef struct Stack {
  struct snode * top;
  int len;
}
Stack;


void init_stack(Stack ** s);
void free_stack(Stack ** s);
int add_stack(Stack * s, void * val);
int isEmptyStack(Stack * s);
void * pop_stack(Stack * s);
void print_stack(Stack * s);
#endif