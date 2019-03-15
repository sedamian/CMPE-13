/*
 * File:   Tree.c
 * Author: sedamian
 *
 * Created on May 28, 2017, 2:07 AM
 */


#include <xc.h>
#include "BOARD.h"
#include "Morse.h"
#include "Tree.h"
#include <plib.h>

/**
 * This function creates a binary tree of a given size given a serialized array of data. All nodes
 * are allocated on the heap via `malloc()` and store the input data in their data member. Note that
 * this parameter is serialized in a top-down, left-to-right order. This means it follows the
 * left-hand-side of the tree to the bottom and then backs it's way back up.
 *
 * So for a tree like:
 *           A
 *        B     C
 *      D   E F   G
 * The data variable is ordered as [A B D E C F G].
 * 
 * This function uses recursion for generating the tree in this top-down, left-right order. This
 * tree, and therefore the input data set, is assumed to be full and balanced therefore each level
 * has 2^level number of nodes in it. If the `data` input is not this size, then this functions
 * behavior is undefined. Since the input data is copied directly into each node, they can
 * originally be allocated on the stack.
 *
 * If allocating memory fails during TreeCreate() then it will return NULL. Additionally, if the
 * creation of either of the subtrees fails within TreeCreate(), it should return NULL. This means
 * that if TreeCreate() returns a non-NULL value, that means that a perfect tree has been created,
 * so all nodes that should exist have been successfully malloc()'d onto the heap.
 *
 * This function does not completely clean up after itself if malloc() fails at any point. Some
 * nodes may be successfully free()d, but a failing TreeCreate() is likely to leave memory in the
 * heap unaccessible.
 *
 * @param level How many vertical levels the tree will have.
 * @param data A serialized array of the character data that will be stored in all nodes. This array
 *              should be of length `2^level - 1`.
 * @return The head of the created tree or NULL if malloc fails for any node.
 */
Node *TreeCreate(int level, const char *data) {

    Node *tree = malloc(sizeof (Node));


    // base case, necessary so that no other code is performed. 
    if (tree == NULL) {
        return NULL;

    }
    tree->data = *data;
    if (level > 1) {


        // This will make it so that the TreeCreate function will call itself 
        // as many times as there are levels. it will then take a step back and 
        // do the right child.
        tree->leftChild = TreeCreate(level - 1, (data + 1));

        //printf ("This is after leftChild %c \n", tree->data);

        // the sata + 1<<(level-1) makes sure that it gets the right char out of
        // *data
        tree->rightChild = TreeCreate(level - 1, (data + (1 << (level - 1))));
        return tree;


        //this sets if not child to NULL
        if (tree->leftChild == NULL) {
            return NULL;
        }
        if (tree->rightChild == NULL) {
            return NULL;
        }
    } else {

        //sets to NUll if empty
        tree->leftChild = NULL;
        tree->rightChild = NULL;
    }


    return tree;
}
