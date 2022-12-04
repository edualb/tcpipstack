/* Forward declaration */
typedef struct _glthread glthread_t;

/* Struct implementation */
struct _glthread {
    struct _glthread *left;
    struct _glthread *right;
};

void NewGLThread(glthread_t *glthread);