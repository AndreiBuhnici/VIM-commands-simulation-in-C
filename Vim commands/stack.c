

#include "stack.h"


void init_stack(Stack ** s) {
  /* Functie pentru eliberarea stivei */
  * s = (Stack * ) malloc(sizeof(Stack));
  ( * s) -> len = 0;
  ( * s) -> top = 0;
}

void free_stack(Stack ** s) {
  /* Funtie de eliberare completa a stivei */
  snode * temp = NULL;
  while (( * s) -> top) {
    free(( * s) -> top -> data);
    temp = ( * s) -> top;
    ( * s) -> top = ( * s) -> top -> next;
    free(temp);
  }
  ( * s) -> len = 0;
}

int add_stack(Stack * s, void * val) {
  /* Functie pentru adaugarea unui element in stiva */
  snode * nou = (snode * ) malloc(sizeof(snode));
  if (nou == NULL)
    return 0;
  nou -> data = val;
  nou -> next = s -> top;
  s -> top = nou;
  s -> len++;
  return 1;
}

int isEmptyStack(Stack * s) {
  /* Functie pentru a verifica daca stiva este goala . Pentru debugging .*/
  return s -> len;
}

void * pop_stack(Stack * s) {
  /* Functia care scoate elementul din stiva si il intoarce , eliberand
     memoria ocupata */
  void * el;
  snode * aux;
  el = s -> top -> data;
  aux = s -> top;
  s -> top = s -> top -> next;
  free(aux);
  s -> len--;
  return el;
}

void print_stack(Stack * s) {
  /* Functie care afiseaza stiva . Pentru debugging */
  snode * current = s -> top;
  printf("\n");
  while (current != NULL) {
    printf(" %s ", (char * ) current -> data);
    current = current -> next;
  }
  printf("\n");
}