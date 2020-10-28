#include <stdio.h>
#include <string.h>

#define AVL_IMPLEMENTATION
#include "../structures/avl.h"

char* scopy(const char* s){
  char* t = malloc(strlen(s));
  strcpy(t, s);
  return t;
}

void tell_me(AvlTree* t, const char* key){
  printf("%s: %d\n", key, (int) avl_search(t, key));
}

int main(int argc, char** argv){

  /* initialize */
  AvlTree t;
  avl_initialize(&t, (avl_comparator_f) strcmp, free);

  /* store data */
  avl_insert(&t, scopy("this is one"), (void*) 1);
  avl_insert(&t, scopy("this is five"), (void*) 5);
  avl_insert(&t, scopy("this is the square of 3"), (void*) 9);

  avl_insert(&t, scopy("this is half of hundred"), (void*) 100);
  /* was the wrong value */
  avl_insert(&t, scopy("this is half of hundred"), (void*) 50);

  /* retrieve data */
  tell_me(&t, "this is half of hundred");
  tell_me(&t, "this is five");
  tell_me(&t, "this is one");
  tell_me(&t, "this is the square of 3");

  /* free tree and keys, do nothing to data (because they are not actually
   * pointers) */
  avl_destroy(&t, NULL);

  return 0;
}
