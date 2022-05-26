#include "Common.h"
#include "StackPrimitives.h"
#include "VFM_Build_Primitives.h"
//
void DefCodeName(char* _name){
int32_t len, pad=0;
  DictNameEntry *here = (DictNameEntry *) &(M.data32[M.HERE]);
  here->EntryLink = M.LATEST;
  M.LATEST = M.HERE;
  strcpy(here->Name, _name);
  here->NameLength = strlen(_name);
  len = here->NameLength + sizeof(here->EntryLink) + sizeof(here->NameLength) + sizeof(here->EntryLength );
  do{ // pad zeros to Name until aligned for int32_t
    if( (len+pad) %4 == 0) break;
    strcat(here->Name, "\0");
    pad++;
  }while(true);
  here->EntryLength = len + pad;
  M.HERE += (here->EntryLength/4);  
}
//
enum RunTypes { _TYPE0, _TYPE1 };
//
void DefCodeHead( int8_t _immediate, int8_t _hidden, int8_t _runtype ){
  DictHeadEntry *here = (DictHeadEntry *) &(M.data32[M.HERE]);
  here->Immediate     = _immediate;
  here->Hidden        = _hidden;
  here->RunType       = _runtype;
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
void Dump(int32_t start, int32_t count){
char buf [16], ac;
union{
  int32_t val;
  char cval[4];
}vals;
  for(int i=start;i<count;i++){
    sprintf(buf," %4.4X %8.8X ",i, M.data32[i]); PrintBuf(buf);
    vals.val = M.data32[i];
    for(int k=3;k>=0;k--){
      ac = vals.cval[k];
      if((ac>32) && (ac<126)){ sprintf(buf,"%1c",ac); PrintBuf(buf); }
      else                   { sprintf(buf,"-"); PrintBuf(buf); }      
    }
    PrintBuf("\n");
  }
}
//
void PrintDictEntries(int32_t thisentry, int32_t count){
char buf[40];  
int32_t i;
 do{
  DictNameEntry *thename = (DictNameEntry *) &(M.data32[thisentry]);
  if(strlen(thename->Name) ==0)break;
  int len = thename->NameLength;
  DictHeadEntry *thehead = (DictHeadEntry *) &(M.data32[thisentry + ((thename->EntryLength)/4)]); 
  DictBodyEntry *thebody = (DictBodyEntry *) &(M.data32[thisentry + ((thename->EntryLength)/4) + sizeof(DictHeadEntry)/4 ] ); 
               for(i=0;i<len;i++){    sprintf(buf,"%c",thename->Name[i]);                   PrintBuf(buf); }
               for(   ;i<32 ;i++){    sprintf(buf," ");                                     PrintBuf(buf); }
                                      sprintf(buf," Link = %4X", thename->EntryLink);       PrintBuf(buf);
  if(thehead->Immediate == false){    sprintf(buf," NotImmediate");                         PrintBuf(buf); }
  if(thehead->Immediate == true) {    sprintf(buf,"    Immediate");                         PrintBuf(buf); }
  if(thehead->Hidden    == false){    sprintf(buf," NotHidden");                            PrintBuf(buf); }
  if(thehead->Hidden    == true) {    sprintf(buf,"    Hidden");                            PrintBuf(buf); }  
                                      sprintf(buf,"    RunType = %d", thehead->RunType);    PrintBuf(buf); 
               len = thehead->CodeCount;
               for(i=0;i<len;i++){    sprintf(buf," fptr[%2.2d] = %8X",i,thebody->code[i]); PrintBuf(buf); }                                      
                                                                                            PrintBuf("\n");
  thisentry += (thename->EntryLength)/4 + sizeof(DictHeadEntry)/4 + thehead->CodeCount;
 }while(--count >0);
}
//
void BuildCodeEntries(int32_t where){
  void (*code[2])(void);
  //
  M.HERE = where;
  //            Name                   Immed  Hidden
  DefCodeName( "DUP"   ); DefCodeHead( false, false, _TYPE1 ); code[0] = _dup;      code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "DROP"  ); DefCodeHead( false, false, _TYPE1 ); code[0] = _drop;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "SWAP"  ); DefCodeHead( false, false, _TYPE1 ); code[0] = _swap;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "OVER"  ); DefCodeHead( false, false, _TYPE1 ); code[0] = _over;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "ROT"   ); DefCodeHead( false, false, _TYPE1 ); code[0] = _prot;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "-ROT"  ); DefCodeHead( false, false, _TYPE1 ); code[0] = _nrot;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "2DROP" ); DefCodeHead( false, false, _TYPE1 ); code[0] = _2drop;    code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "2DUP"  ); DefCodeHead( false, false, _TYPE1 ); code[0] = _2dup;     code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "2SWAP" ); DefCodeHead( false, false, _TYPE1 ); code[0] = _2swap;    code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "?DUP"  ); DefCodeHead( false, false, _TYPE1 ); code[0] = _qdup;     code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "1+"    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _onep;     code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "1-"    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _onen;     code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "4+"    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _fourp;    code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "4-"    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _fourn;    code[1] = _fth_exit; DefCodeBody(2, code);    
  DefCodeName( "+"     ); DefCodeHead( false, false, _TYPE1 ); code[0] = _add;      code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "-"     ); DefCodeHead( false, false, _TYPE1 ); code[0] = _sub;      code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "*"     ); DefCodeHead( false, false, _TYPE1 ); code[0] = _mul;      code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "/MOD"  ); DefCodeHead( false, false, _TYPE1 ); code[0] = _smod;     code[1] = _fth_exit; DefCodeBody(2, code);   
  DefCodeName( "="     ); DefCodeHead( false, false, _TYPE1 ); code[0] = _equal;    code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "<>"    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _nequal;   code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "<"     ); DefCodeHead( false, false, _TYPE1 ); code[0] = _less;     code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( ">"     ); DefCodeHead( false, false, _TYPE1 ); code[0] = _greatr;   code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "<="    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _lesseq;   code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( ">="    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _greatreq; code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "0="    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _zeroeq;   code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "0<>"   ); DefCodeHead( false, false, _TYPE1 ); code[0] = _zeroneq;  code[1] = _fth_exit; DefCodeBody(2, code); 
  DefCodeName( "0<"    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _zerolth;  code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "0>"    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _zerogth;  code[1] = _fth_exit; DefCodeBody(2, code);  
  DefCodeName( "0<="   ); DefCodeHead( false, false, _TYPE1 ); code[0] = _zltheq;   code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "0>="   ); DefCodeHead( false, false, _TYPE1 ); code[0] = _zerogth;  code[1] = _fth_exit; DefCodeBody(2, code); 
  DefCodeName( "AND"   ); DefCodeHead( false, false, _TYPE1 ); code[0] = _bitsand;  code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "OR"    ); DefCodeHead( false, false, _TYPE1 ); code[0] = _bitsor;   code[1] = _fth_exit; DefCodeBody(2, code);   
  DefCodeName( "XOR"   ); DefCodeHead( false, false, _TYPE1 ); code[0] = _bitsxor;  code[1] = _fth_exit; DefCodeBody(2, code);
  DefCodeName( "INVERT"); DefCodeHead( false, false, _TYPE1 ); code[0] = _bitsnot;  code[1] = _fth_exit; DefCodeBody(2, code);  
}
