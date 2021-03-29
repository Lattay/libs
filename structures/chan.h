/*--------------------------------------------------------------------------*\
 * chan.h
 *
 * Lock-less single-producer single-consumer channel.
 * Warning: Operations are only thread-safe if there is only one producer
 * thread and one consumer thread.
 *
 * 2021 Mar 26, by Théo Cavignac (theo.cavignac@gmail.com)
 *
 * By default this file is only a header.
 * The implementation of functions is added only if CHAN_IMPL is defined.
 * Jump to CHAN_IMPL to go to the start of implementation.
\*--------------------------------------------------------------------------*/
#ifndef CHAN_H
#define CHAN_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct chan_state chan_state;

chan_state* chan_new_state(size_t max_nodes);
chan_state* chan_share_state(chan_state * s);
void chan_free(chan_state * s);

/* Push a piece of data onto the channel if
 * there is free space and return true.
 * If there is no space, return false
 */
bool chan_push(chan_state * s, void* data);

/* If there is data on the channel, pop a piece of data,
 * have data point to it and return true.
 * If there is nothing, return false
 */
bool chan_pop(chan_state * s, void** data);

#endif
#ifdef CHAN_IMPL

typedef struct chan_area {
  size_t max_nodes;
  size_t head;
  size_t tail;
  size_t ref_count;
  void* nodes[];
} chan_area;

typedef struct chan_state {
  chan_area* area;
} chan_state;

static inline bool free_space(chan_area * area) {
  size_t t = area->tail;
  size_t h = area->head;

  if (t == h) {
    return true;
  } else if (t < h) {
    t += area->max_nodes;
  }

  return t - h < area->max_nodes;
}

static inline bool ready(chan_area * area) {
  return area->tail != area->head;
}

bool chan_pop(chan_state * s, void** data) {
  size_t c = s->area->head;
  size_t head = (c + 1) % s->area->max_nodes;
  if (ready(s->area)) {
    *data = NULL;
    return false;
  }
  s->area->head = head;
  *data = s->area->nodes[c];
  return true;
}

bool chan_push(chan_state * s, void* data) {
  size_t c = s->area->tail;
  size_t tail = (c + 1) % s->area->max_nodes;
  if (free_space(s->area)) {
    return false;
  }
  s->area->tail = tail;
  s->area->nodes[c] = data;
  return true;
}

chan_state* chan_new_state(size_t max_nodes) {
  chan_area* a = malloc(max_nodes * sizeof(void *) + 4 * sizeof(size_t));
  chan_state* s = malloc(sizeof(struct chan_state));

  if (!a) {
    s->area = NULL;
  } else {
    a->max_nodes = max_nodes;
    a->head = 0;
    a->tail = 0;
    a->ref_count = 1;
    s->area = a;
  }

  return s;
}

chan_state* chan_share_state(chan_state * s) {
  ++s->area;
  chan_state* s2 = malloc(sizeof(struct chan_state));
  s2->area = s->area;
  return s2;
}

void chan_free(chan_state * s) {
  if (!--s->area->ref_count) {
    free(s->area);
  }
  free(s);
}
#endif
