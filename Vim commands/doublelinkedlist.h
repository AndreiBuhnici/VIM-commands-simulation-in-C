#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node {
  char data;
  struct node * next, * prev;
}
node;

typedef struct TDoubleLinkedList {
  node * head;
  node * tail;
  int len;
}
TDoubleLinkedList;


void init(TDoubleLinkedList ** list);
int add(TDoubleLinkedList * list, int n, char data);
void print_char_list(TDoubleLinkedList * list);
void rev_print_char_list(TDoubleLinkedList * list);
void rev_fprintf_char_list(TDoubleLinkedList * list);
void del(TDoubleLinkedList * list, int n);
void free_list(TDoubleLinkedList ** list);
#endif