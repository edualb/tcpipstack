/* Forward declaration */
typedef struct _glthread glthread_t;

/* Struct implementation */
struct _glthread {
    struct _glthread *left;
    struct _glthread *right;
};

void gluethread_NewGLThread(glthread_t *glthread);
void gluethread_AddNext(glthread_t *source, glthread_t *new);
glthread_t* gluethread_GetNext(glthread_t *source);