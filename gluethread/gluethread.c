#include "gluethread.h"

#include<stdio.h>

// NewGLThread return an empty Doubly Linked List 
void gluethread_NewGLThread(glthread_t *glthread) {
    glthread->left = NULL;
    glthread->right = NULL;
}

// AddNext include a new item in the beginning of the source Doubly Linked List
void gluethread_AddNext(glthread_t *source, glthread_t *new) {
    if (source->right == NULL) {
        source->right = new;
        new->left = source;
        return;
    }

    glthread_t *tmp = source->right;
    tmp->left = new;
    new->right = tmp;
    new->left = source;
    source->right = new;
}

// GetNext return the next item from the Doubly Linked List
glthread_t* gluethread_GetNext(glthread_t *source) {
    if (source == NULL) {
        return NULL;
    }
    return source->right;
}