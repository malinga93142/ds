#ifndef _AVL_TREE_H
#define _AVL_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define LH +1
#define EH  0
#define RH -1

/**
 * structure for node.
 * a node contains left pointer, data, balance, and
 * right pointer.
 */
typedef struct node NODE;


/**
 * structure for AVL Tree.
 * a tree node contains total counts, a node pointer, and 
 * function pointer.
 */
typedef struct AVL_TREE AVL_TREE;



// ADT's
/**
 * Creating AVL Tree
 * @param function pointer
 * 
*/
AVL_TREE *AVL_Create(int (*compare)(void*, void*));
/**
 * Destroying an AVL Tree.
 * @param pointer root node
 */
AVL_TREE *AVL_Destroy(AVL_TREE*);

/**
 * @brief inserting data pointer in tree
 * @param AVL_TREE root node.
 * @param data data to be insert
 */
bool AVL_Insert(AVL_TREE*, void*);

/**
 * @brief deleting data pointer from the tree
 * @param AVL_TREE root node.
 * @param data_ptr data to be deleted.
 */
bool AVL_Delete(AVL_TREE*, void*);

void* AVL_Retrieve(AVL_TREE*, void*);

void AVL_Traverse(AVL_TREE*, void (*process)(void*));

int AVL_Count(AVL_TREE*);
bool AVL_Empty(AVL_TREE*);
bool AVL_FULL(AVL_TREE*);


// private functions

#ifdef __INTERNAL
static NODE* _insert(AVL_TREE* tree, NODE* root, NODE* newptr, bool *taller);

static NODE* _delete(AVL_TREE* tree, NODE* root, void* data, bool *shorter, bool *success);

static void* _retrieve(AVL_TREE* tree, void* keyptr, NODE* root);

static void _traversal(NODE* root, void (*process)(void* dataptr));

static void _destroy(NODE* root);

static NODE* rotateLeft(NODE* root);
static NODE* rotateRight(NODE* root);

static NODE* insLeftBal(NODE* root, bool *taller);
static NODE* insRightBal(NODE* root, bool *taller);

static NODE* dltLeftBal(NODE* root, bool *shorter);
static NODE* dltRightBal(NODE* root, bool *shorter);
#endif
#endif