typedef struct{
  int32_t  Link;
  bool     Immediate;
  bool     Hidden;  
  int16_t  RunType;  
  void     (*code)(void);
  void     (*done)(void);
  char     Name[32]; 
}DictCodeEntry;
//
void DefCodeEntry(char*, bool, bool, int16_t, void (*)(void));
void BuildCodeEntries(int32_t);
void Dump(int32_t, int32_t);
void PrintDictEntries(int32_t);
