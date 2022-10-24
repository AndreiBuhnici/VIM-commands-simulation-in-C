#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "stack.h"

#include "doublelinkedlist.h"


Stack * undo, * redo;


char * itoa(int val, char * res)
/* Functie care converteste un int in string */
{
  char * p = res, * p1 = res, aux, * cif = "0123456789";
  int k;
  do {
    k = val;
    val = val / 10;
    * p = cif[k - val * 10];
    p = p + 1;
  } while (val);
  p = p - 1;
  /*	In cazul in care avem un numar de mai multe cifre , cum p retine numarul 
  	invers , cu acest while vom readuce numarul la forma initiala  */
  while (p1 < p) {
    aux = * p;
    * p = * p1;
    p = p - 1;
    * p1 = aux;
    p1 = p1 + 1;
  }
  return res;
}



int verif_change(TDoubleLinkedList * list) {
  /* Functie pentru a verifica trecerea de la introducere text la comenzi */
  node * current = list -> tail;
  if (current -> data == ':' && current -> prev -> data == ':' && current -> prev -> prev -> data == 'i')
    return 1;
  else
    return 0;
}

int nr_cif(int n) {
  /* Functie pentru aflarea numarului de cifre al unui intreg */
  int k = 0;
  if (n == 0)
    k = 1;
  while (n != 0) {
    k++;
    n = n / 10;
  }
  return k;
}

int gl(int n, TDoubleLinkedList * list) {
  /* Functie separata de cea de comanda pentru goto line , pentru apelari mai 
     usoare cand este nevoie .*/
  node * x = list -> tail;
  int k = 0, pos = 0;
  /* Din moment ce am retinut lista dublu inlantuita astfel si nu ca o matrice,
     voi decrementa numarul liniei mereu cand dam peste un new line. */
  while (n != 0) {
    if (x -> data == '\n') {
      n--;
    }
    x = x -> prev;
    k++;
  }
  pos = list -> len - k;
  return pos;
}

int dl(int n, int pos, TDoubleLinkedList * list) {
  /* Functie separata de cea de comanda pentru delete line */
  node * x = list -> tail;
  int k = 0;
  /* Se foloseste de acelasi rationament ca si la gl , iar dupa incepe stergerea
     pana la new line */
  if (n != 0) {
    while (n != 0) {
      if (x -> data == '\n') {
        n--;
      }
      x = x -> prev;
      k++;
    }
    pos = list -> len - k;
    del(list, pos);
    while (x -> data != '\n' && x != NULL) {
      x = x -> prev;
      del(list, pos - 1);
      pos--;
    }
    pos--;
  } else

  {
    if (pos == 0)
      pos = list -> len;

    while (k != list -> len - pos) {
      x = x -> prev;
      k++;
      if (x -> prev == NULL)
        break;
    }

    while (x -> data != '\n') {
      x = x -> prev;
      del(list, pos - 1);
      pos--;
    }
    del(list, pos - 1);
    pos--;
  }

  return pos;
}

void copiez(TDoubleLinkedList * list, TDoubleLinkedList * aux);

void comanda(TDoubleLinkedList * list, TDoubleLinkedList * aux) {
  /* Functie principala care ruleaza toate comenzile */
  node * current = aux -> tail, * ucurr = aux -> tail, * x = list -> tail;
  int lin = 0, col = 0, i = 0, k = 0, nr = 0, start = 0, poz = 0;
  char * comm, * auxs;
  static int pos;
  /* Am alocat dinamic un vector de caractere pentru a putea retine comenzile
     simplu in cele 2 stive de undo si redo. In vector se copiaza cuvantul
     din lista aux(care este comanda) caracter cu caracter */
  comm = (char * ) calloc(aux -> len + 1, sizeof(char));
  while (ucurr != NULL) {

    comm[i] = ucurr -> data;
    ucurr = ucurr -> prev;
    i++;
  }
  if (comm[0] == 'g' && comm[1] == 'l') {
    /* Prelucare pentru stocarea lui gl in stive , in functie de pozitia anterioara
       si cea curenta */
    comm = (char * ) realloc(comm, aux -> len + 1 + nr_cif(pos));
    comm[i] = ' ';
    itoa(pos, comm + i + 1);
    comm[i + nr_cif(pos)] = '\0';
    lin = aux -> head -> data - 48 - 1;
    pos = gl(lin, list);
  } else

  if (comm[0] == 'd' && comm[1] == 'l') {
    /* Prelucare epntru stocarea lui dl in stive , in functie de textul sters si 
       pozitia de unde a fost sters */
    if (i > 2) {
      poz = gl(comm[3] - 48 - 1, list);
    } else {
      if (pos == 0)
        poz = list -> len;
      poz = pos;

    }

    poz = list -> len - poz;

    if (poz != list -> len) {
      while (poz != 0) {
        x = x -> prev;
        poz--;
      }
      if (x -> next != NULL && x -> next -> data != '\n') {
        while (x -> next -> data != '\n') {
          x = x -> next;
          pos++;
        }

      }

    }

    while (x -> data != '\n')

    {
      comm = (char * ) realloc(comm, aux -> len + i);
      comm[i] = x -> data;
      x = x -> prev;
      i++;
    }
    comm[i] = '\0';
    auxs = (char * ) malloc(strlen(comm) - 1);
    strcpy(auxs, comm + 2);

    if (aux -> head -> data == 'l')
      lin = 0;
    else
      lin = aux -> head -> data - 48 - 1;
    pos = dl(lin, pos, list);
    itoa(pos, comm + 2);
    if (lin != 0) {
      comm = (char * ) realloc(comm, strlen(comm) + strlen(auxs) + nr_cif(lin) + 1);
      strcat(comm, auxs + nr_cif(lin) + 1);
    } else {
      comm = (char * ) realloc(comm, strlen(comm) + strlen(auxs) + 1);
      strcat(comm, auxs);
    }
    free(auxs);
  } else

  if (comm[0] == ':' && comm[1] == ':' && comm[2] == 'i') {
    /* Prelucrarea comenzi de inserare text pentru a putea fi inserat in cele
       2 sitve , depinzand de pozitie */

    comm[i] = '\0';
    if (pos == 0 && comm[3] != '\0' && comm[3] != ' ') {
      del(aux, aux -> len - 1);
      del(aux, aux -> len - 1);
      del(aux, aux -> len - 1);

      current = aux -> tail;
      copiez(list, aux);

      itoa(pos, comm + i);
    } else
    if (pos != 0 && comm[3] != '\0' && comm[3] != ' ') {
      del(aux, aux -> len - 1);
      del(aux, aux -> len - 1);
      del(aux, aux -> len - 1);
      current = aux -> tail;
      poz = list -> len - pos;
      while (poz != 0) {
        x = x -> prev;
        poz--;
      }
      if (x -> prev != NULL) {
        del(aux, 0);
        current = aux -> tail;
      }

      while (current != NULL) {
        add(list, pos, current -> data);
        current = current -> prev;
      }
      current = aux -> tail;

      itoa(pos, comm + i - 1);
    }
  } else
    comm[i] = '\0';

  /* Verificarea comenzi pentru a putea fi introdusa in stiva de undo */
  if (strcmp(comm, "u") != 0 && strcmp(comm, "r") != 0 && strcmp(comm, "s") != 0 && strcmp(comm, "q") != 0)

  {
    add_stack(undo, comm);
    if (comm[0] == ':') {
      return;
    }
  }

  if (current -> data == 'u') {
    /* In cazul in care comanda este chiar undo vom extrage ultima comanda
       introdusa si vom avea iar mai multe cazuri in functie de comanda */
    free(comm);
    comm = pop_stack(undo);

    if (comm[0] == 'g' && comm[1] == 'l') {
      poz = 3;
      while (comm[poz] != ' ')
        poz++;
      pos = atoi(comm + poz);
      add_stack(redo, comm);
    }

    if (comm[0] == 'd' && comm[1] == 'l') {
      pos = atoi(comm + 2);
      poz = list -> len - pos;
      while (poz != 0) {
        x = x -> prev;
        poz--;
      }
      for (i = 2 + nr_cif(pos); i < strlen(comm); i++) {
        add(list, pos, comm[i]);
      }
      add(list, pos, '\n');
      add_stack(redo, comm);
    }

    if (comm[0] == ':' && comm[1] == ':' && comm[2] == 'i') {
      add_stack(redo, comm);
      comm = pop_stack(undo);
      do {
        i = 0;
        poz = 0;
        k = 0;
        while (comm[i] != '\n') {
          i++;
        }
        poz = atoi(comm + i - 1);
        k = list -> len - poz;
        while (k != 0 && x -> prev != NULL) {
          x = x -> prev;
          k--;
        }
        if (x -> prev != NULL)
          x = x -> next;
        if (x -> data == '\n') {
          x = x -> next;
          del(list, poz);
        }

        while (x -> data == comm[i - 1] && comm[i - 2] != ':') {
          x = x -> next;
          del(list, poz);
          i--;
        }
        add_stack(redo, comm);
        comm = pop_stack(undo);
        x = list -> tail;
      } while (comm[0] == ':' && comm[1] == ':' && comm[2] == 'i' && comm[3] != '\0' && comm[3] != ' ');
      add_stack(redo, comm);
    }
    return;
  }

  if (current -> data == 'r' && current -> prev == NULL) {
    /* In cazul in care comanda este chiar cea de redo extragem , ca si la 
       ceea de undo , ultima comanda introdusa si in functie de comanda vom readuce
       lista la forma anterioara (inaintea comenzii de undo) */
    free(comm);
    comm = pop_stack(redo);

    if (comm[0] == 'g' && comm[1] == 'l') {
      pos = atoi(comm + 3);
      pos = gl(pos - 1, list);
    }

    if (comm[0] == 'd' && comm[1] == 'l') {
      pos = atoi(comm + 2);
      poz = list -> len - pos;

      while (poz != 0) {
        if (x -> data == '\n')
          lin++;
        x = x -> prev;
        poz--;
      }
      dl(lin - 1, 0, list);
    }

    if (comm[0] == ':' && comm[1] == ':' && comm[2] == 'i') {
      add_stack(undo, comm);
      comm = pop_stack(redo);
      do {
        i = 0;
        k = 0;
        poz = 0;
        while (comm[i] != '\n')
          i++;
        poz = atoi(comm + i - 1);
        k = list -> len - poz;
        while (k != 0 && x -> prev != NULL) {
          x = x -> prev;
          k--;
        }
        if (x -> prev != NULL)
          x = x -> next;
        i = 3;
        while (comm[i] != '\n') {

          add(list, poz, comm[i]);
          x = x -> next;
          i++;
        }
        add(list, poz, '\n');
        add_stack(undo, comm);

        comm = pop_stack(redo);
        x = list -> tail;

      } while (comm[0] == ':' && comm[1] == ':' && comm[2] == 'i' && comm[3] != '\0' && comm[3] != ' ');
      add_stack(undo, comm);
      return;
    }

    add_stack(undo, comm);
    return;
  }

  if (current -> data == 'q') {
    /* Cazul comenzii de quit in care eliberam toata memoria alocata si iesim cu
       exit code-ul 0 , adica succes */
    free_list( & list);
    free_list( & aux);
    free_stack( & undo);
    free_stack( & redo);
    free(undo);
    free(redo);
    free(comm);
    free(list);
    free(aux);
    exit(0);
  }

  if (current -> data == 's') {
    /* Cazul comenzii de save in care doar scriem statutul actual al listei in 
       fisierul de out */
    rev_fprintf_char_list(list);
    free(comm);
    return;
  }

  if (current -> data == 'g' && current -> prev -> data == 'c')
  /* Pozitia unde se afla cursorul la momentul actual. Se fac parcurgeri 
     in functie de linie si coloana si se decrementeaza */
  {
    node * y;

    if (current -> prev -> prev -> prev -> prev == NULL) {
      lin = 0;
      col = aux -> head -> data - 48 - 1;
    } else {
      lin = aux -> head -> data - 48 - 1;
      col = aux -> head -> next -> next -> data - 48 - 1;
    }
    if (lin != 0) {
      while (lin != 0) {
        if (x -> data == '\n') {
          lin--;
        }
        x = x -> prev;
        k++;
      }
      while (col != 0) {
        x = x -> prev;
        col--;
        k++;
      }
      pos = list -> len - k - 1;
      return;
    }

    if (lin == 0) {
      lin = pos - 1;

      if (lin < 0)
        pos = list -> len - col;
      else {
        y = list -> head;
        while (lin > 0 || y -> next -> data != '\n') {
          y = y -> next;
          lin--;
          k++;
        }
        while (col != 0) {
          y = y -> prev;
          col--;
          k--;
        }
        pos = k;
      }
      return;
    }
  }

  if (current -> data == 'b')
    /* Cazul comenzii de backspace care functioneaza exact ca si functia de delete
       a unui nod , deci este ,pur si simplu, apelata */
    del(list, pos);

  if (current -> data == 'd' && (current -> prev == NULL || current -> prev -> data == '\n' || current -> prev -> data == ' ')) {
    /* Cazul comenzii de delete , care sterge cuvinte inainte si if-ul de sus 
       verificand cazul in care se specifica sau nu cate caractere se doreste
       sa fie sterse. Se realizeaza cu parcurgere si functia delete al unui nod */
    if (aux -> head -> data == 'd')
      nr = 1;
    else
      nr = aux -> head -> data - 48;
    if (pos == 0)
      pos = list -> len;
    while (k != list -> len - pos) {
      x = x -> prev;
      k++;
      if (x -> prev == NULL)
        break;
    }
    while (nr != 0) {
      if (x -> data == '\n') {
        x = x -> prev;
        pos--;
      } else {
        x = x -> prev;
        del(list, pos - 1);
        pos--;
        nr--;
      }
    }
    if (x -> prev -> data == '\n') {
      del(list, pos - 2);
      pos--;
    }
    return;
  }

  if (current -> data == 'r' && current -> prev -> data == 'e') {
    /* Cazul comenzii de replace pe un singur cuvant . Se realizeaza prin 
       parcurgere, comparatie , stergere si inserare. De asemenea , functia
       face replace primului cuvant de la pozitia cursorului */
    node * cuv = current -> prev -> prev -> prev;

    if (pos == 0)
      pos = list -> len;
    while (k != list -> len - pos) {
      x = x -> prev;
      k++;
      if (x -> prev == NULL)
        break;
    }
    k = 0;
    while (cuv -> data != ' ' && cuv != NULL && cuv -> data != '\n')
    /* presupunem ca ar exista cuvantul dupa cursor, altfel nu ar avea sens */
    {
      if (x -> data == cuv -> data) {
        x = x -> prev;
        cuv = cuv -> prev;
        k++;

        pos--;
      } else
      if (x -> data != cuv -> data && cuv -> next -> data != ' ') {
        while (k != 0) {
          cuv = cuv -> next;
          k--;
        }
      } else
      if (x -> data != cuv -> data && cuv -> next -> data == ' ') {
        while (x -> data != cuv -> data) {
          if (x -> prev == NULL)
            return;
          pos--;
          x = x -> prev;
        }

      }

    }
    start = pos + k;
    /* ne orientam dupa lungimea cuvantului , inceputul sau si sfarsitul */
    k = start - pos;
    while (pos != start) {
      x = x -> next;
      pos++;
    }
    while (k != 0) {
      del(list, pos - 1);
      pos--;
      k--;
    }
    cuv = cuv -> prev;
    while (cuv != NULL) {
      add(list, pos, cuv -> data);
      cuv = cuv -> prev;
    }
    return;
  }

  if (current -> data == 'r' && current -> prev -> data == 'a') {
    /* Comanda de replace all care se bazeaza pe acelasi principiu ca si cea
       de replace , dar fiind facuta pe tot textul de la inceput pana la sfarsit,
       executand aceleasi comenzi intr-un while */
    node * cuv = current -> prev -> prev -> prev, * new = current -> prev -> prev -> prev;
    pos = 0;
    if (pos == 0)
      pos = list -> len;
    while (k != list -> len - pos) {
      x = x -> prev;
      k++;
      if (x -> prev == NULL)
        break;
    }
    while (x -> prev != NULL) {

      k = 0;
      while (cuv -> data != ' ' && cuv != NULL && cuv -> data != '\n')
      /* presupunem ca ar exista cuvantul dupa cursor, altfel nu ar avea sens */
      {
        if (x -> data == cuv -> data) {
          x = x -> prev;
          cuv = cuv -> prev;
          k++;
          pos--;
        } else
        if (x -> data != cuv -> data && cuv -> next -> data != ' ') {
          while (k != 0) {
            cuv = cuv -> next;
            k--;
          }
        } else
        if (x -> data != cuv -> data && cuv -> next -> data == ' ') {
          while (x -> data != cuv -> data) {
            if (x -> prev == NULL)
              return;
            pos--;
            x = x -> prev;
          }

        }

      }
      start = pos + k;
      /* ne orientam dupa lungimea cuvantului , inceputul sau si sfarsitul */
      k = start - pos;
      while (pos != start) {
        x = x -> next;
        pos++;
      }

      while (k != 0) {
        del(list, pos - 1);
        pos--;
        k--;
      }
      cuv = cuv -> prev;
      while (cuv != NULL) {
        add(list, pos, cuv -> data);
        cuv = cuv -> prev;
      }
      cuv = new;
      x = list -> tail;
      k = 0;
      while (k != list -> len - pos) {
        x = x -> prev;
        k++;
        if (x -> prev == NULL)
          break;
      }
    }
    return;
  }

  if (current -> data == 'd' && current -> prev != NULL && current -> prev -> data == 'w') {
    /* Comanda de delete word care este foarte asemanatoare cu cea de replace word,
       dar lipsindu-i liniile de adaugare a noilor caractere */
    node * cuv = current -> prev -> prev -> prev;

    if (pos == 0)
      pos = list -> len;
    while (k != list -> len - pos) {
      x = x -> prev;
      k++;
      if (x -> prev == NULL)
        break;
    }

    k = 0;
    while (1)
    /* presupunem ca ar exista cuvantul dupa cursor, altfel nu ar avea sens */
    {
      if (x -> data == cuv -> data) {
        x = x -> prev;
        cuv = cuv -> prev;
        k++;
        pos--;
      } else
      if (x -> data != cuv -> data && cuv -> next -> data != ' ') {
        while (k != 0) {
          cuv = cuv -> next;

          k--;
        }
      } else
      if (x -> data != cuv -> data && cuv -> next -> data == ' ') {
        while (x -> data != cuv -> data) {
          if (x -> prev == NULL)
            return;
          pos--;
          x = x -> prev;
        }

      }
      if (cuv == NULL)
        break;

    }
    start = pos + k;
    /* ne orientam dupa lungimea cuvantului , inceputul sau si sfarsitul */
    k = start - pos;
    while (pos != start) {
      x = x -> next;
      pos++;
    }

    while (k != 0) {
      del(list, pos - 1);
      pos--;
      k--;
    }
    return;
  }

  if (current -> data == 'd' && current -> prev != NULL && current -> prev -> data == 'a') {
    /* Comanda de delete all , care sterge fiecare aparitie a unui cuvant dat.
       De asemenea , si aceasta , seamana foarte mult cu comanda de replace all,
       dar lipsind linile de adaugare de caractere */
    node * cuv = current -> prev -> prev -> prev, * new = current -> prev -> prev -> prev;
    pos = 0;
    if (pos == 0)
      pos = list -> len;
    while (k != list -> len - pos) {
      x = x -> prev;
      k++;
      if (x -> prev == NULL)
        break;
    }
    while (x -> prev != NULL) {
      k = 0;
      while (1)
      /* presupunem ca ar exista cuvantul dupa cursor, altfel nu ar avea sens */
      {

        if (x -> data == cuv -> data) {
          x = x -> prev;
          cuv = cuv -> prev;
          k++;
          pos--;
        } else
        if (x -> data != cuv -> data && cuv -> next -> data != ' ') {
          while (k != 0) {
            cuv = cuv -> next;
            k--;
          }
        } else
        if (x -> data != cuv -> data && cuv -> next -> data == ' ') {
          while (x -> data != cuv -> data) {
            if (x -> prev == NULL)
              return;
            pos--;
            x = x -> prev;
          }

        }
        if (cuv == NULL)
          break;

      }
      start = pos + k;
      /* ne orientam dupa lungimea cuvantului , inceputul sau si sfarsitul */
      k = start - pos;
      while (pos != start) {
        x = x -> next;
        pos++;
      }

      while (k != 0) {
        del(list, pos - 1);
        pos--;
        k--;
      }
      cuv = new;
      x = list -> tail;
      k = 0;
      while (k != list -> len - pos) {
        x = x -> prev;
        k++;
        if (x -> prev == NULL)
          break;
      }
      if (x -> data == '\n') {
        x = x -> next;
        del(list, pos - 1);
      }
    }

    return;
  }

} /* sfarsitul functiei comanda */

void copiez(TDoubleLinkedList * list, TDoubleLinkedList * aux) {
  /* Functie care copiaza dintr-o lista in alta . In cazul nostru, din comanda
     in lista cu textul */
  node * current2 = aux -> head;
  int i = 0;
  while (current2 != NULL) {
    add(list, i, current2 -> data);
    current2 = current2 -> next;
    i++;
  }
  free(current2);
}

int main() {
  TDoubleLinkedList * list;
  FILE * fp;
  int ok = 0;
  TDoubleLinkedList * cuv;
  char c;
  init( & list);
  init( & cuv);
  init_stack( & undo);
  init_stack( & redo);
  /* Initializarea celor 2 list duble inlantuite si a celor 2 stive de undo si
     redo */
  fp = fopen("editor.in", "r+");
  if (fp == NULL)
    printf("Eroare la deschiderea fisierului.\n");

  while ((c = fgetc(fp)) != EOF) {
    /* Citim caracter cu caracter pana la sfarsitul fisierului , iar la aparitia 
       caracterului newline vom face o verificare a listei cuv pentru a vedea 
       daca trecem la comenzi sau nu.*/
    if (c == '\n') {
      if (verif_change(cuv) == 1) {
        comanda(list, cuv);
        ok = (ok - 1) * (-1);
        free_list( & cuv);
        free(cuv);
        init( & cuv);
      }

      if (ok == 0 && cuv -> head != NULL) {
        /* In cazul in care ajungem iar la newline si ok==0 , inseamna ca introducem
           text si vom merge cu cuvantul in comanda unde se va copia in lista
           cu textul */
        add(cuv, 0, c);
        add(cuv, cuv -> len, 'i');
        add(cuv, cuv -> len, ':');
        add(cuv, cuv -> len, ':');
        comanda(list, cuv);
        free_list( & cuv);
        free(cuv);
        init( & cuv);
      }

      if (ok == 1 && cuv -> head != NULL) {
        /* In cazul de newline si ok==1 , inseamna ca introducem comenzi , deci se 
           vor duce in comanda si vor fi executate pe text */
        if (cuv -> head -> data == 'q')
          fclose(fp);
        comanda(list, cuv);
        free_list( & cuv);
        free(cuv);
        init( & cuv);
      }

    } else
      /* Iar daca nu am ajuns inca la newline copiam tot in cuv si il vom elibera
         dupa fiecare newline pentru simplitate */
      add(cuv, 0, c);

  }
  if (cuv -> head -> data == 'q')
  /* Conditie bonus in cazul in care dupa comanda q nu se afla un newline si 
     iesim din while -ul de mai sus (am introdus-o pentru anumite erori de pe
     checker-ul local) */
  {
    fclose(fp);
    comanda(list, cuv);
  }

  return 0;

}