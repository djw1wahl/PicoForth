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
#ifdef __cplusplus
}
#endif
//
typedef struct{
  union{
    uint32_t data32[MEMSIZE];
    uint8_t  data08[MEMSIZE*4];
  };
  int32_t  rstack[STACKSIZE]; // do not change STACKSIZE
  int32_t  pstack[STACKSIZE];
  float    fstack[STACKSIZE];  
  int32_t  TMPA, TMPB, TMPC, P, WP;
  int8_t   ridx, pidx, fidx, pad;
  char     tib_buf[TIBSIZE];
}MemoryImage;
//
extern MemoryImage M;
//
typedef struct{
  char     Name[31];
  char     nPad;
  int8_t   NameLength;
  int8_t   Immediate;
  int8_t   Hidden;
  int8_t   Pad;
  int32_t  *code;
}DictHeadEntry;
//
