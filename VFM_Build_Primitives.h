#define IMMED 0x2000
#define HIDEN 0x4000
#define NADAZ 0x0000
#define LENMASK 0x1F
//
typedef struct{
  int32_t  Link;
  union{
    int32_t   Len;        // len in lower 5 bits, 0x1F
    int32_t   Immediate;  // Immediate == bit 5,  0x20
    int32_t   Hidden;     // Hidden    == bit 6,  0x40
  };
  void     (*cfa)(void);
  void     (*code)(void);
  void     (*done)(void);
  char     Name[32]; 
}DictCodeEntry;
//
void DefCodeEntry(char*, int32_t, int32_t, void (*)(void), void (*)(void));
void BuildCodeEntries(int32_t);
void Dump(int32_t, int32_t);
void PrintDictEntries(void);
