#ifndef MYADD_H
#define MYADD_H

#include <stdlib.h>

// #define MY_MEM_MODE
#define MY_MEM_MODE_V2

// #define MY_AUDIO_MODE_V1

#define MY_VIDEO_MODE_V1

#ifdef MY_MEM_MODE
#define MY_MEM_START 0xfc00
#else
#define MY_MEM_START 0
#endif

#define MY_ALLOC_TEXT(dest,source) \
    if (dest) { \
        if (strlen(dest) < strlen(source)) { \
            free(dest); \
            dest = NULL; \
        } \
      } \
      if (!dest) { \
        dest = (char*)malloc(strlen(source)+1); \
      } \
      strcpy(dest,source);

#define MY_ALLOC

#ifdef MY_ALLOC
/* speed, 1=8bit, 4=32bit */
#define MY_MEM_NEW_FAST(a,b) (a*)my_special_alloc(1, 1, b)
#define MY_MEM_NEW_SLOW(a,b) (a*)my_special_alloc(0, 1, b)

#define MY_MEM_ALLOC_FAST(a,b) (a*)my_special_alloc(1, 1, b)
#define MY_MEM_ALLOC_FAST_EXT(a,b,c) (a*)my_special_alloc(1, c, b)
#define MY_MEM_ALLOC_SLOW(a,b) (a*)my_special_alloc(0, 1, b)
#define MY_MEM_ALLOC_SLOW_EXT(a,b,c) (a*)my_special_alloc(0, c, b)

#define MY_MEM_NEW_FREE(a) my_special_alloc_free(a)
#define MY_MEM_ALLOC_FREE(a) my_special_alloc_free(a)
#else
#define MY_MEM_NEW_FAST(a,b) new a[b]
#define MY_MEM_NEW_SLOW(a,b) new a[b]

#define MY_MEM_ALLOC_FAST(a,b) (a*)malloc(b)
#define MY_MEM_ALLOC_FAST_EXT(a,b,c) (a*)malloc(b)
#define MY_MEM_ALLOC_SLOW(a,b) (a*)malloc(b)
#define MY_MEM_ALLOC_SLOW_EXT(a,b,c) (a*)malloc(b)

#define MY_MEM_NEW_FREE(a) delete[] a
#define MY_MEM_ALLOC_FREE(a) free(a)
#endif

#ifdef __cplusplus
extern "C" {
#endif

//extern void *my_special_alloc(UBYTE speed, UBYTE bytes, ULONG size);
extern void *my_special_alloc(unsigned char speed, unsigned char bytes, unsigned long size);
extern void my_special_alloc_free(void *p);

#ifdef __cplusplus
}
#endif

#endif