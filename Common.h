#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
//
#define FLASH_SECTORS_AVAILABLE 256
#define FLASH_SECTORS_OFFSET 1024 * 1024
#define PAGES_PER_SECTOR 16
#define PAGE_SIZE 256
#define MEMSIZE 4096 * 4
#define TIBSIZE 64
#define STACKSIZE 256
#//
#ifdef __cplusplus
extern "C" {
#endif
  void    serial_putchar(char);
  int32_t serial_getchar(void);
  void    PrintBuf(char*);
  void    PrintNumber(int32_t);
#ifdef __cplusplus
}
#endif
//
typedef struct{
  union{
    uint32_t data32[MEMSIZE];
    uint8_t  data08[MEMSIZE*4];
  };
  int32_t  Page_Buf[PAGE_SIZE];
  int32_t  rstack[STACKSIZE]; // do not change STACKSIZE
  int32_t  pstack[STACKSIZE];
  float    fstack[STACKSIZE];  
  int32_t  iA32,   iB32,  iC32;
  int32_t  *pA32, *pB32, *pC32;  
  int8_t   iA8,   iB8,  iC8,  iD8;
  int8_t   *pA8, *pB8, *pC8, *pD8;
  int32_t  STATE, LATEST, HERE, BASE, S0;
  int32_t  WP, PicoIDbuf;
  int8_t   ridx, pidx, fidx, pad;
  char     tib_buf[TIBSIZE];
}MemoryImage;
//
extern MemoryImage M;
