#ifndef __LOOPBUF_H__ 
#define __LOOPBUF_H__ 

#include <windows.h>
 
#ifdef __cplusplus 
extern "C"{ 
#endif 
 
#define BB_ERROR 0
#define BB_SUCCESS 1
#define SEMA_MAX_CNT 1
 
typedef int (*pfunc) (int argc, void *argv); 
 
typedef struct 
{ 
    unsigned char    *buf; 
    unsigned int     len; 
    unsigned int     read; 
    unsigned int     write; 
    unsigned int     max; 
    pfunc   PV_init; 
    pfunc   P; 
    pfunc   V; 
    HANDLE  hSem; 
    char    name[64]; 
    INT     int_level; 
}_loopbuf; 
 
#define     EM_LOOPBUF_NONE_LOCK        0 
#define     EM_LOOPBUF_THREAD_INT       1 
#define     EM_LOOPBUF_THREAD_THREAD    2 
 
int loop_init(_loopbuf *loopbuf,char *name,unsigned int max,char mode); 
int loop_write(_loopbuf *loopbuf,const char *buf,unsigned int len); 
int loop_read(_loopbuf *loopbuf,char *buf,unsigned int len); 
unsigned int loop_size(_loopbuf *loopbuf); 
int loop_clr_end(_loopbuf *loopbuf,char *buf); 
int loop_reset(_loopbuf *loopbuf); 
 
#ifdef __cplusplus 
} 
#endif 
 
#endif 
