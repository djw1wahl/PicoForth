#define IMMED   0x01
#define HIDEN   0x01
#define NADAZ   0x00
#define LENMASK 0x1F
//
typedef struct{
  int32_t  Link;
  int8_t   Len;               // len in lower 5 bits, 0x1F
  int8_t   Immediate;         // Immediate == bit 5,  0x20
  int8_t   Hidden;            // Hidden    == bit 6,  0x40
  int8_t   NumParam;          // count of parameters after Name
  int32_t  pfa;               // points beyond Name, where parameters are stored by <comma> or InsertParameters()
  char     Name[32]; 
}DictEntry;
//
void DefHeader(char*, int32_t, int32_t);
void InsertParameter(int32_t);
void BuildCodeEntries(int32_t);
void Dump(int32_t);
void PrintDictEntries(void);
