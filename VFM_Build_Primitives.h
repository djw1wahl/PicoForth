typedef struct{
  int32_t  EntryLink;    // Links the Dict List
  int8_t   NameLength;
  int8_t   EntryLength;  // final length of this Entry after padding Name  
  char     Name[32];
}DictNameEntry;
//
typedef struct{
  int8_t   RunType;
  int8_t   Immediate;
  int8_t   Hidden;
  int8_t   CodeCount; // count of Dict >Body< Entry code pointers()
}DictHeadEntry;
//
typedef struct{
  void(*code[4])(void);
}DictBodyEntry;
//
void DefCodeName(char*);
void DefCodeHead( int8_t, int8_t, int8_t );
void DefCodeBody( int32_t, void(*f[])(void));
void BuildCodeEntries(int32_t);
void Dump(int32_t, int32_t);
void PrintDictEntries(int32_t, int32_t);
