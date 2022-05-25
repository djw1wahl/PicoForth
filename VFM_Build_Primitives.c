#include "Common.h"
#include "StackPrimitives.h"
#include "VFM_Build_Primitives.h"
//
void DefCodeName(char* _name){
int32_t pad;
  DictNameEntry *here = (DictNameEntry *) &(M.data32[M.HERE]);
  strcpy(here->Name, _name);
  here->NameLength = strlen(_name);
  pad = here->NameLength +2; // 2 bytes + actuall NameLength
  do{
    if( pad %4 == 0) break;
    here->Name[(pad++)-1] = (char)0x00;
  }while(true);
  here->EntryLength = pad;
  M.HERE += (pad/4);  
}
//
void DefCodeHead( int8_t _immediate, int8_t _hidden, void(*fptr)(void) ){
  DictHeadEntry *here = (DictHeadEntry *) &(M.data32[M.HERE]);
  here->Immediate = _immediate;
  here->Hidden = _hidden;
  here->code = fptr;
  M.HERE += sizeof(DictHeadEntry)/4;
}
//
void DefCodeBody(int32_t cnt, void(*f[])(void)){
  DictBodyEntry  *body   = (DictBodyEntry *) &(M.data32[M.HERE]);
  DictHeadEntry  *head   = (DictHeadEntry *) &(M.data32[M.HERE - sizeof(DictHeadEntry)/4]);  
  head->CodeCount = cnt;
  for(int i=0;i<cnt;i++){ body->code[i] = *f[i]; }
  M.HERE += head->CodeCount;
}
//
void RunCodes(void){
  PrintBuf("This is RunCodes\n");
}
//
void Dump(int32_t start, int32_t count){
char buf [16];
  for(int i=start;i<count;i++){
    sprintf(buf," %4.4X %8.8X\n",i, M.data32[i]); PrintBuf(buf);
  }
}
//
void PrintDictEntries(int32_t thisentry, int32_t count){
char buf[40];  
int32_t i;
 do{
  DictNameEntry *thename = (DictNameEntry *) &(M.data32[thisentry]);
  int len = thename->NameLength;
  DictHeadEntry *thehead = (DictHeadEntry *) &(M.data32[thisentry + ((thename->EntryLength)/4)]); 
  DictBodyEntry *thebody = (DictBodyEntry *) &(M.data32[thisentry + ((thename->EntryLength)/4) + sizeof(DictHeadEntry)/4 ] ); 
               for(i=0;i<len;i++){    sprintf(buf,"%c",thename->Name[i]);                   PrintBuf(buf); }
               for(   ;i<32 ;i++){    sprintf(buf," ");                                     PrintBuf(buf); }
  if(thehead->Immediate == false){    sprintf(buf," NotImmediate");                         PrintBuf(buf); }
  if(thehead->Immediate == true) {    sprintf(buf,"    Immediate");                         PrintBuf(buf); }
  if(thehead->Hidden    == false){    sprintf(buf," NotHidden");                            PrintBuf(buf); }
  if(thehead->Hidden    == true) {    sprintf(buf,"    Hidden");                            PrintBuf(buf); }  
                                      sprintf(buf,"    cptr = %8X", thehead->code);         PrintBuf(buf); 
               len = thehead->CodeCount;
               for(i=0;i<len;i++){    sprintf(buf," fptr[%2.2d] = %8X",i,thebody->code[i]); PrintBuf(buf); }                                      
                                                                                            PrintBuf("\n");
  thisentry += (thename->EntryLength)/4 + sizeof(DictHeadEntry)/4 + thehead->CodeCount;
  if(M.data32[thisentry] ==0)break;
 }while(--count >0);
}
//
void BuildCodeEntries(void){
  void (*code[2])(void);
  //                                   Immed  Hidden
  DefCodeName( "DUP"   ); DefCodeHead( false, false, RunCodes ); code[0] = _dup;      code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "DROP"  ); DefCodeHead( false, false, RunCodes ); code[0] = _drop;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "SWAP"  ); DefCodeHead( false, false, RunCodes ); code[0] = _swap;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "OVER"  ); DefCodeHead( false, false, RunCodes ); code[0] = _over;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "ROT"   ); DefCodeHead( false, false, RunCodes ); code[0] = _prot;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "-ROT"  ); DefCodeHead( false, false, RunCodes ); code[0] = _nrot;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "2DROP" ); DefCodeHead( false, false, RunCodes ); code[0] = _2drop;    code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "2DUP"  ); DefCodeHead( false, false, RunCodes ); code[0] = _2dup;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "2SWAP" ); DefCodeHead( false, false, RunCodes ); code[0] = _2swap;    code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "?DUP"  ); DefCodeHead( false, false, RunCodes ); code[0] = _qdup;     code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "1+"    ); DefCodeHead( false, false, RunCodes ); code[0] = _onep;     code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "1-"    ); DefCodeHead( false, false, RunCodes ); code[0] = _onen;     code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "4+"    ); DefCodeHead( false, false, RunCodes ); code[0] = _fourp;    code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "4-"    ); DefCodeHead( false, false, RunCodes ); code[0] = _fourn;    code[1] = _fth_exit; DefCodeBody(2, code);    
  DefCodeName( "+"     ); DefCodeHead( false, false, RunCodes ); code[0] = _add;      code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "-"     ); DefCodeHead( false, false, RunCodes ); code[0] = _sub;      code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "*"     ); DefCodeHead( false, false, RunCodes ); code[0] = _mul;      code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "/MOD"  ); DefCodeHead( false, false, RunCodes ); code[0] = _smod;     code[1] = _fth_exit; DefCodeBody(2, code);   
  DefCodeName( "="     ); DefCodeHead( false, false, RunCodes ); code[0] = _equal;    code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "<>"    ); DefCodeHead( false, false, RunCodes ); code[0] = _nequal;   code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "<"     ); DefCodeHead( false, false, RunCodes ); code[0] = _less;     code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( ">"     ); DefCodeHead( false, false, RunCodes ); code[0] = _greatr;   code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "<="    ); DefCodeHead( false, false, RunCodes ); code[0] = _lesseq;   code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( ">="    ); DefCodeHead( false, false, RunCodes ); code[0] = _greatreq; code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "0="    ); DefCodeHead( false, false, RunCodes ); code[0] = _zeroeq;   code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "0<>"   ); DefCodeHead( false, false, RunCodes ); code[0] = _zeroneq;  code[1] = _fth_exit; DefCodeBody(2, code); 
  DefCodeName( "0<"    ); DefCodeHead( false, false, RunCodes ); code[0] = _zerolth;  code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "0>"    ); DefCodeHead( false, false, RunCodes ); code[0] = _zerogth;  code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "0<="   ); DefCodeHead( false, false, RunCodes ); code[0] = _zltheq;   code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "0>="   ); DefCodeHead( false, false, RunCodes ); code[0] = _zerogth;  code[1] = _fth_exit; DefCodeBody(2, code); 
  DefCodeName( "AND"   ); DefCodeHead( false, false, RunCodes ); code[0] = _bitsand;  code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "OR"    ); DefCodeHead( false, false, RunCodes ); code[0] = _bitsor;   code[1] = _fth_exit; DefCodeBody(2, code);   
  DefCodeName( "XOR"   ); DefCodeHead( false, false, RunCodes ); code[0] = _bitsxor;  code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "INVERT"); DefCodeHead( false, false, RunCodes ); code[0] = _bitsnot;  code[1] = _fth_exit; DefCodeBody(2, code);  
}
