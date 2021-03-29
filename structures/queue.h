/*--------------------------------------------------------------------------*\
 * Queue implementation by Théo Cavignac (theo.cavignac@gmail.com)
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * 
 * See <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * This file implements one of the simplest yet most useful data structure.
 * Anyone can write this, but I don't want to have to anymore.
 * 
 * By default this file is only a header.
 * The implementation of functions is added only if Q_IMPLEMENTATION
 * is defined.
 * Jump to Q_IMPLEMENTATION to go to the start of implementation.
\*--------------------------------------------------------------------------*/

#ifndef Q_H
#define Q_H

#define Q_ALLOC(variable, type) variable = (type *)malloc(sizeof(type))
#define Q_FREE(variable) free(variable)
#include <stdlib.h> /* for malloc() */

typedef struct QElem {
  void* data;
  struct QElem* next;
} QElem;

typedef struct {
  QElem* head;
  QElem* tip;
} Queue;

void q_push(Queue* , void* data);
void q_unshift(Queue* , void* data);
void* q_shift(Queue* );
void* q_empty(Queue* );

#endif

#ifdef Q_IMPLEMENTATION

// Insert data at the begining
void q_push(Queue* queue, void* data) {
  QElem* elem = Q_ALLOC(QElem);
  elem->data = data;
  elem->next = queue->head;
  queue->head = elem;
}

// Insert data at the end
void q_unshift(Queue* queue, void* data) {
  QElem* elem = Q_ALLOC(QElem);
  elem->data = data;
  if (!queue->head) {
    elem->next = NULL;
    queue->head = elem;
  } else {
    queue->tip->next = elem;
  }
  queue->tip = elem;
}

// Remove data from the begining and return it
void* q_shift(Queue* queue) {
  if (!queue->head) {
    return NULL;
  }
  QElem* elem = queue->head;
  queue->head = elem->next;
  void* data = elem->data;
  Q_FREE(elem);
  return data;
}

bool q_empty(Queue* queue) {
  return !queue->head;
}

#endif // Q_IMPLEMENTATION
