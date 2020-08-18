/*--------------------------------------------------------------------------*\
 *  AVL tree implementation by Kent "ethereal" Williams-King and is
 *  hereby released into the public domain. Do what you wish with it.
 *
 *  No guarantees as to the correctness of the implementation are provided.
 *
 *  Original project https://github.com/etherealvisage/avl
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * 2020 Aug 18, Modifications by Théo Cavignac (theo.cavignac@gmail.com)
 *
 * Packaging as a single file header.
 *
 * Several style modifications:
 * Get rid of AVL_NAME macro because it hurt redability for minimal benefit.
 * struct names starts with Avl and are named with CamelCase.
 * functions types use snake_case and have a _f suffix
 * remove bracket-less if
 * make key managment contract coherent:
 * - user MUST allocate keys
 * - user MUST NOT destroy key
 * - avl_ MUST destroy keys when they are not needed anymore
 *
 * Documentation is just before each function in header part (just below).
 *
 * By default this file is only a header.
 * The implementation of functions is added only if AVL_IMPLEMENTATION
 * is defined.
 * Jump to AVL_IMPLEMENTATION to go to the start of implementation.
\*--------------------------------------------------------------------------*/


#ifndef AVL_H
#define AVL_H

/* memory allocation macros, change as necessary */
#define AVL_ALLOC(variable, type) variable = (type *)malloc(sizeof(type))
#define AVL_FREE(variable) free(variable)
#include <stdlib.h> /* for malloc() */

typedef int (*avl_comparator_f)(void *key1, void *key2);
typedef void (*avl_key_destructor_f)(void *key);
typedef void (*avl_node_visitor_f)(void *key, void *data);

typedef struct AvlTreeNode {
  struct AvlTreeNode *left, *right;
  int depth;

  void *key;
  void *data;
} AvlTreeNode;

typedef struct {
  AvlTreeNode *root;
  avl_comparator_f comparator;
  avl_key_destructor_f destructor;
} AvlTree;

/*--------------------------------------------------------------------------*\
 *  NAME:
 *      avl_initialize() - initialize a new tree
 *  DESCRIPTION:
 *      Initialize a tree. The user have to own the memory corresponding to the
 *      tree. It should be cleand with avl_destroy.
 *  ARGUMENTS:
 *      tree        - a pointer to the tree to initialize
 *      comparator  - a avl_comparator_f function pointer ((void*, void*) -> int) to
 *                    compare keys
 *      destructor  - a avl_key_destructor_f function pointer (void* -> void) to
 *                    destroy keys
\*--------------------------------------------------------------------------*/
void avl_initialize(AvlTree *tree,
    avl_comparator_f comparator, avl_key_destructor_f destructor);

/*--------------------------------------------------------------------------*\
 *  NAME:
 *      avl_destroy() - destroy a tree
 *  ARGUMENTS:
 *      tree        - a pointer to the tree to destroy
 *      visitor     - a avl_node_visitor_f function pointer ((void *key, void *data) -> void)
 *                    Applied on each key-value pair before destroying it. Use
 *                    it for your own cleanup of the data.
 *                    visitor should NOT free the key.
\*--------------------------------------------------------------------------*/
void avl_destroy(AvlTree *tree,
    avl_node_visitor_f visitor);

/*--------------------------------------------------------------------------*\
 *  NAME:
 *      avl_search() - search for a key and eventually return the data
 *  DESCRIPTION:
 *      Search for the key and if it is found, return a pointer to the data.
 *      Return NULL if nothing have been found.
 *  EFFICIENCY:
 *      O(log(n))
\*--------------------------------------------------------------------------*/
void *avl_search(AvlTree *tree, void *key);

/*--------------------------------------------------------------------------*\
 *  NAME:
 *      avl_insert() - insert a key-value pair
 *  DESCRIPTION:
 *      If the key is already present, replace the value with the new data
 *      and return a pointer to the old data.
 *      Else insert the data and return NULL.
 *      Anyway the key memory is no longer yours, it may be freed or used,
 *      consider that you should forget about it.
 *  EFFICIENCY:
 *      O(log(n))
\*--------------------------------------------------------------------------*/
void *avl_insert(AvlTree *tree, void *key, void *data);

/*--------------------------------------------------------------------------*\
 *  NAME:
 *      avl_remove() - remove a key-value pari
 *  DESCRIPTION:
 *      Search for the key and if it is found, remove it from the tree and
 *      return a pointer to the data.
 *      Return NULL if nothing have been found.
 *  EFFICIENCY:
 *      O(log(n))
\*--------------------------------------------------------------------------*/
void *avl_remove(AvlTree *tree, void *key);

/*--------------------------------------------------------------------------*\
 *  NAME:
 *      avl_tree_depth() - return the depth of the tree
 *  EFFICIENCY:
 *      O(1)
\*--------------------------------------------------------------------------*/
int avl_tree_depth(AvlTree *tree);

/*
 * The following are basic avl_comparator_f that can be used when initializing the tree
 */
int avl_ptrcmp(void *key1, void *key2);
int avl_intcmp(void *key1, void *key2);
int avl_ulongcmp(void *key1, void *key2);

/*
 * The following is a basic avl_node_visitor_f that free data
 */
void avl_free_data(void *key, void *data);

#endif

#ifdef AVL_IMPLEMENTATION
/* required definitions */
#ifndef NULL
#define NULL ((void *)0)
#endif

/* recursive destruction helper */
static void avl_destroy_helper(AvlTree *tree,
    AvlTreeNode *node, avl_node_visitor_f visitor);
/* recursive insertion helper */
static void *avl_insert_helper(AvlTree *tree,
    AvlTreeNode **node, void *key, void *data);
/* recursive removal helper, finds the appropriate node to remove */
static void *avl_remove_helper(AvlTree *tree,
    AvlTreeNode **node, void *key);
/* recursive removal helper, updates tree depths after node swap */
static void avl_remove_depth_helper(AvlTreeNode *ptr);

#define AVL_LEFT 0
#define AVL_RIGHT 1
/* rotates a node and its left/right child as appropriate (left=0, right=1) */
static void avl_rotate(AvlTreeNode **ptr, int which);

/* performs rotations to appropriately rebalance a node and its children */
static void avl_rebalance(AvlTreeNode **ptr);
/* calculates how out-of-balance a node is (>0 if left deeper) */
static int avl_balance_factor(AvlTreeNode *ptr);
/* recalculates the depth of a node */
static void avl_update_depth(AvlTreeNode *ptr);

void avl_initialize(AvlTree *tree, avl_comparator_f comparator, avl_key_destructor_f destructor) {

  tree->comparator = comparator;
  tree->destructor = destructor;
  tree->root = NULL;
}

void avl_destroy(AvlTree *tree, avl_node_visitor_f visitor) {

  avl_destroy_helper(tree, tree->root, visitor);
}

static void avl_destroy_helper(AvlTree *tree,
    AvlTreeNode *node, avl_node_visitor_f visitor) {

  if(node == NULL){
    return;
  }

  visitor(node->key, node->data);
  tree->destructor(node->key);
  avl_destroy_helper(tree, node->left, visitor);
  avl_destroy_helper(tree, node->right, visitor);

  AVL_FREE(node);
}

void *avl_search(AvlTree *tree, void *key) {
  AvlTreeNode *node = tree->root;
  int cmp;
  while(node) {
    cmp = tree->comparator(key, node->key);
    if(cmp == 0){
      return node->data;
    } else if(cmp < 0) {
      node = node->left;
    } else /* if(cmp > 0) */ {
      node = node->right;
    }
  }
  return NULL;
}

void *avl_insert(AvlTree *tree, void *key, void *data) {
  return avl_insert_helper(tree, &tree->root, key, data);
}

static void *avl_insert_helper(AvlTree *tree,
    AvlTreeNode **node, void *key, void *data) {

  int cmp;
  void *ret;

  /* if the search leads us to an empty location, then add the new node.
     rebalancing, if required, will be handled by the parent call in the
     recursion. */
  if(!*node) {
    AVL_ALLOC(*node, AvlTreeNode);
    (*node)->depth = 1;
    (*node)->key = key;
    (*node)->data = data;
    (*node)->left = (*node)->right = NULL;

    return NULL;
  }

  cmp = tree->comparator(key, (*node)->key);
  if(cmp == 0) {
    /* if we find a node with the same value, then replace the contents.
       no rebalancing is required, but will be checked by parent recursion
       call nonetheless. */
    void *old = (*node)->data;
    (*node)->data = data;
    /* we don't need the new key any more. */
    if(tree->destructor){
      tree->destructor(key);
    }
    return old;
  } else if(cmp < 0) {
    ret = avl_insert_helper(tree, &(*node)->left, key, data);
  } else /*if(cmp > 0)*/ {
    ret = avl_insert_helper(tree, &(*node)->right, key, data);
  }

  /* check, and rebalance the current node, if necessary */
  avl_rebalance(node);
  /* ensure the depth of this node is correct */
  avl_update_depth(*node);

  return ret;
}

void *avl_remove(AvlTree *tree, void *key) {
  return avl_remove_helper(tree, &tree->root, key);
}

static void *avl_remove_helper(AvlTree *tree,
    AvlTreeNode **node, void *key) {

  int cmp;
  void *ret;

  /* if we didn't find the node, then, well . . . */
  if(!*node){
    return NULL;
  }

  cmp = tree->comparator(key, (*node)->key);

  if(cmp < 0){
    ret = avl_remove_helper(tree, &(*node)->left, key);
  } else if(cmp > 0){
    ret = avl_remove_helper(tree, &(*node)->right, key);
  } else /* if(cmp == 0) */ {
    /* node found. */
    AvlTreeNode **y, *p = NULL;

    ret = (*node)->data;
    if(tree->destructor){
      tree->destructor((*node)->key);
    }

    /* complicated case */
    if((*node)->left && (*node)->right) {
      /* use maximum node in left subtree as the replacement */
      y = &(*node)->left;
      while((*y)->right) y = &(*y)->right;

      /* copy contents out */
      (*node)->key = (*y)->key;
      (*node)->data = (*y)->data;

      /* replace the found node with its left child: if there is no left
         child, this will replace it with NULL */
      p = (*y)->left;
      AVL_FREE(*y);
      *y = p;

      /* ensure all the depths in the left subtree are correct. */
      avl_remove_depth_helper((*node)->left);
    } else if((*node)->left) {
      /* no right subtree, so replace this node with the left subtree */
      p = (*node)->left;
      AVL_FREE(*node);
      *node = p;
    } else if((*node)->right) {
      /* no left subtree, so replace this node with the right subtree */
      p = (*node)->right;
      AVL_FREE(*node);
      *node = p;
    } else {
      /* no children at all, i.e. a leaf */
      AVL_FREE(*node);
      *node = NULL;
    }
  }

  /* if the node was replaced, ensure the depth is correct and that
     everything is balanced */
  if(*node) {
    avl_update_depth(*node);
    avl_rebalance(node);
  }

  return ret;
}

static void avl_remove_depth_helper(AvlTreeNode *ptr) {
  if(ptr) {
    avl_remove_depth_helper(ptr->right);
    avl_update_depth(ptr);
  }
}

static void avl_rebalance(AvlTreeNode **node) {
  int delta = avl_balance_factor(*node);

  /* two rotation directions */
  if(delta == 2) {
    if(avl_balance_factor((*node->left) < 0)) {
      avl_rotate(&(*node)->left, AVL_LEFT);
    }
    avl_rotate(node, AVL_RIGHT);
  } else if(delta == -2) {
    if(avl_balance_factor((*node->right) > 0)) {
      avl_rotate(&(*node)->right, AVL_RIGHT);
    }
    avl_rotate(node, AVL_LEFT);
  }
}

static void avl_rotate(AvlTreeNode **node, int dir) {
  AvlTreeNode *ch;

  /* standard tree rotations */
  if(dir == 0) {
    ch = (*node)->right;

    (*node)->right = (*node)->right->left;
    ch->left = *node;
    avl_update_depth(*node);
    *node = ch;
  }
  else {
    ch = (*node)->left;

    (*node)->left = (*node)->left->right;
    ch->right = *node;
    avl_update_depth(*node);
    *node = ch;
  }
  avl_update_depth(*node);
}

static int avl_balance_factor(AvlTreeNode *ptr) {
  int delta = 0;
  if(ptr->left){
    delta = ptr->left->depth;
  }
  if(ptr->right){
    delta -= ptr->right->depth;
  }
  return delta;
}

static void avl_update_depth(AvlTreeNode *ptr) {
  ptr->depth = 1;
  if(ptr->left){
    ptr->depth = ptr->left->depth;
  }
  if(ptr->right && ptr->depth < ptr->right->depth) {
    ptr->depth = ptr->right->depth;
  }
  ptr->depth ++;
}

int avl_tree_depth(AvlTree *tree) {
  if(tree->root){
    return tree->root->depth;
  }
  return 0;
}

int avl_ptrcmp(void *key1, void *key2) {
  if(key1 < key2){
    return -1;
  } else if(key1 > key2){
    return 1;
  } else {
    return 0;
  }
}

int avl_intcmp(void *key1, void *key2) {
  int val1 = *(int *)key1;
  int val2 = *(int *)key2;
  if(val1 < val2){
    return -1;
  } else if(val1 > val2) {
    return 1;
  } else {
    return 0;
  }
}

int avl_ulongcmp(void *key1, void *key2) {
  unsigned long val1 = *(int *)key1;
  unsigned long val2 = *(int *)key2;
  if(val1 < val2){
    return -1;
  } else if(val1 > val2) {
    return 1;
  } else {
    return 0;
  }
}

void avl_free_data(void *key, void *data) {
  AVL_FREE(data);
}
#endif  // AVL_IMPLEMENTATION
