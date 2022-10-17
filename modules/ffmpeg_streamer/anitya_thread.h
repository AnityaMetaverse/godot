#ifndef ANITYA_THREAD_H
#define ANITYA_THREAD_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include "pthread.h"
#else
    #include <pthread.h>
#endif
#endif