#define IMMED     0x01
#define HIDEN     0x02
#define NADAZ     0x00
#define LENMASK   0x1F
//
typedef struct{
  int32_t  Link;
  int8_t   Len;               // len in lower 5 bits, 0x1F
  int8_t   Flags;             // Immediate == 0x01, Hidden == 0x02
  int16_t  This;              // The Index of this word
  int32_t  cfa;               // points beyond Name, where parameters are stored by <comma> or InsertParameters()
  char     Name[32]; 
}DictEntry;
//
void DefHeader(char*, int32_t, int32_t);
void Insert(int32_t);
void InsertWord(char*);
void BuildCodeEntries(int32_t);
void BuildWordEntries(int32_t);
//
void Dump(int32_t);
DictEntry* ViewEntry(char*);
void PrintDictEntries(char*, char*);
