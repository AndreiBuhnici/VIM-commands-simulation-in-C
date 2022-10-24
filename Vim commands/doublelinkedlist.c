

#include "doublelinkedlist.h"


void init(TDoubleLinkedList ** list) {
  /* functie de initializare a listei dublu inlantuite */
  * list = (TDoubleLinkedList * ) malloc(sizeof(TDoubleLinkedList));
  ( * list) -> len = 0;
  ( * list) -> head = NULL;
  ( * list) -> tail = NULL;

}

int add(TDoubleLinkedList * list, int n, char data) {
  /* functie care adauga un nod in lista dublu inlantuita la o pozitie data */
  node * nou, * current1 = list -> head, * current2;
  int i = 0;
  nou = (node * ) malloc(sizeof(node));
  nou -> prev = NULL;
  nou -> next = NULL;
  nou -> data = data;
  if (n < 0 || n > list -> len)
    return -1;

  if (list -> head == NULL) {
    list -> head = nou;
    list -> tail = nou;
    list -> len++;
    return 1;
  } else
  if (n == 0) {
    nou -> next = list -> head;
    list -> head -> prev = nou;
    list -> head = nou;
    list -> len++;
    return 1;
  } else
  if (n == list -> len) {
    nou -> prev = list -> tail;
    list -> tail -> next = nou;
    list -> tail = nou;
    list -> len++;
    return 1;
  } else
    /*de asemenea , in functie de pozitie , parcurgerea incepe de la final sau
      de la inceput pentru nodurile interioare */
    if (n <= list -> len / 2) {
      while (current1 != NULL && i < n - 1) {
        current1 = current1 -> next;
        i++;
      }
      nou -> next = current1 -> next;
      current1 -> next -> prev = nou;
      current1 -> next = nou;
      nou -> prev = current1;
      list -> len++;
      return 1;
    } else
  if (n > list -> len / 2) {
    current2 = list -> tail;
    while (current2 != NULL && i < list -> len - n - 1) {
      current2 = current2 -> prev;
      i++;
    }
    nou -> next = current2;
    nou -> prev = current2 -> prev;
    current2 -> prev -> next = nou;
    current2 -> prev = nou;
    list -> len++;
    return 1;

  }
  return 0;
}

void print_char_list(TDoubleLinkedList * list) {
  /* functie care afiseaza lista normal (invers de cum ar trebui afisata) */
  node * current = list -> head;
  printf("\n");
  if (current == NULL)
    printf("Lista goala\n");
  while (current != NULL) {
    printf("%c", current -> data);
    current = current -> next;
  }
  printf("\n");
}

void rev_print_char_list(TDoubleLinkedList * list) {
  /*	functie care afiseaza lista inversata , folosita pentru debugging */
  node * current = list -> tail;
  printf("\n");
  if (current == NULL)
    printf("Lista goala\n");
  while (current != NULL) {
    printf("%c", current -> data);
    current = current -> prev;
  }
  printf("\n");
}

void rev_fprintf_char_list(TDoubleLinkedList * list) {
  /* functie care scrie in fisier lista inversata , deoarece a asa a fost retinuta */
  node * current = list -> tail;
  FILE * fp;
  fp = fopen("editor.out", "w");
  if (current == NULL)
    fprintf(fp, "Lista goala\n");
  while (current != NULL) {
    fprintf(fp, "%c", current -> data);
    current = current -> prev;
  }
  fclose(fp);
}

void del(TDoubleLinkedList * list, int n) {
  /* numaratoare incepe de la 0 , primul nod este pe pozitia 0
     deci ultimul este pe pozitia list->len-1
     functia de stergere a unui nod din lista dublu inlantuita de la o pozitie data */
  node * current = list -> head, * sec_current = list -> tail;
  node * next;
  int i;
  if (list -> head == NULL) {
    printf("Lista este goala\n");
    return;
  }

  if (n == 0) {
    list -> head = current -> next;
    current -> next -> prev = current -> prev;
    free(current);
    list -> len--;
    return;
  }

  if (n == list -> len - 1) {
    list -> tail = sec_current -> prev;
    sec_current -> prev -> next = NULL;
    free(sec_current);
    list -> len--;
    return;
  }

  for (i = 0; current != NULL && i < n - 1; i++)
    current = current -> next;

  if (current == NULL || current -> next == NULL)
    return;
  next = current -> next -> next;
  next -> prev = current -> next -> prev;
  free(current -> next);
  list -> len--;
  current -> next = next;
  return;
}

void free_list(TDoubleLinkedList ** list) {
  /* Functie pentru eliberarea completa a listei dublu inlantuite */
  node * temp = NULL;
  while (( * list) -> head != NULL) {
    temp = ( * list) -> head;
    ( * list) -> head = ( * list) -> head -> next;
    ( * list) -> len--;
    free(temp);
  }

}