#include "gluethread.h"

#include<stdio.h>

// NewGLThread return an empty Doubly Linked List 
void NewGLThread(glthread_t *glthread) {
    glthread->left = NULL;
    glthread->right = NULL;
}