/*--------------------------------------------------------------------------*\
 * Stack implementation by Théo Cavignac (theo.cavignac@gmail.com)
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
 * The implementation of functions is added only if STK_IMPLEMENTATION
 * is defined.
 * Jump to STK_IMPLEMENTATION to go to the start of implementation.
\*--------------------------------------------------------------------------*/

#ifndef STK_H
#define STK_H

#define STK_ALLOC(variable, type) variable = (type *)malloc(sizeof(type))
#define STK_FREE(variable) free(variable)
#include <stdlib.h> /* for malloc() */
#include <stdbool.h>

typedef struct StkElem {
  void* data;
  struct StkElem* next;
} StkElem;

typedef struct {
  StkElem* head;
} Stack;

void stk_push(Stack*, void* data);
void* stk_pop(Stack*);
bool stk_empty(Stack*);

#endif

#ifdef STK_IMPLEMENTATION
void stk_push(Stack* stack, void* data){
  StkElem* elem = STK_ALLOC(StkElem);
  elem->data = data;
  elem->next = stack->head;
  stack->head = elem;
}

void* stk_pop(Stack* stack){
  if(!stack->head){
    return NULL;
  }
  StkElem* elem = stack->head;
  stack->head = elem->next;
  void* data = elem->data;
  STK_FREE(elem);
  return data;
}

bool stk_empty(Stack* stack){
  return !stack->head;
}

#endif  // STK_IMPLEMENTATION
