//
typedef struct{
  int8_t   NameLength;
  int8_t   EntryLength;  
  char     Name[32];
}DictNameEntry;
//
typedef struct{
  int8_t   Immediate;
  int8_t   Hidden;
  int16_t  CodeCount; // count of DictBodyEntry code pointers()
  void     (*code)(void);
}DictHeadEntry;
//
typedef struct{
  void(*code[4])(void);
}DictBodyEntry;
//
void DefCodeName(char*);
void DefCodeHead( int8_t, int8_t, void(*)(void) );
void DefCodeBody( int32_t, void(*f[])(void));
void BuildCodeEntries(void);
void Dump(int32_t, int32_t);
void PrintDictEntries(int32_t, int32_t);
