#include "Common.h"
#include "StackPrimitives.h"
#include "VFM_Build_Primitives.h"
//
void ClearMemory(void){ 
int i;  
  for(i=0;i<MEMSIZE;i++)  { M.data32[i] = 0; } 
  for(i=0;i<STACKSIZE;i++){ M.rstack[i] = 0; }
  for(i=0;i<STACKSIZE;i++){ M.pstack[i] = 0; }
  for(i=0;i<STACKSIZE;i++){ M.fstack[i] = 0.0; }  
  M.iA32 =0;
  M.iB32 =0; 
  M.iC32 =0;
  M.STATE=0;
  M.LATEST=0;
  M.HERE=0;
  M.BASE=0;  
  M.WP     =0;
  M.ridx   =0;
  M.pidx   =0;
  M.fidx   =0;
  M.pad    =0;
  for(i=0;i<TIBSIZE;i++)  { M.tib_buf[i] = (char) 0; }  
}
//
void StackDump(int32_t *stkptr, int32_t count, char* str){
char buf[32];
int i,j;
//
  PrintBuf("--- ");
  PrintBuf(str);
  PrintBuf("\n");
  j = M.pidx;
  for(i=0;i<8;i++){
    if(j==0){
      sprintf(buf," %8c",'|');
    }else{
      sprintf(buf," %8c",' ');
    }
    PrintBuf(buf);
    j--;
  }
  PrintBuf("\n");
  //
  for(i=0;i<STACKSIZE;i+=8){
    for(j=0;j<8;j++){
      sprintf(buf," %8X ",stkptr[i+j]); PrintBuf(buf);
    }
    PrintBuf("\n");
    if(i+j >= count)break;
  }
}
/*
#define INIT_STK   for(int i=0;i<STACKSIZE;i++){ M.pstack[i] = 0; } M.pidx   =0; \
                  _drop(); PushP =10; PushP =20; PushP =30; PushP =40; 
//
void TestStkOps(void){
  //
  INIT_STK
  StackDump(M.pstack, 8, "Default Stack for eack test.");
  //
  INIT_STK
  _swap();
  StackDump(M.pstack, 8, "swap");
  //
  INIT_STK
  _dup();
  StackDump(M.pstack, 8, "dup");
  //
  INIT_STK
  _drop();  
  StackDump(M.pstack, 8, "drop");  
  //
  INIT_STK
  PushP =0;
  StackDump(M.pstack, 8, "0");  
  //
  INIT_STK
  _over();
  StackDump(M.pstack, 8, "over");  
  //
  INIT_STK
  _prot();
  StackDump(M.pstack, 8, "rot");
  //
  INIT_STK
  _nrot();
  StackDump(M.pstack, 8, "-rot");
  //
  INIT_STK
  _2dup();
  StackDump(M.pstack, 8, "2dup");
  //
  INIT_STK
  _2swap();
  StackDump(M.pstack, 8, "2swap");
  //
  INIT_STK
  PushP = 0;
  _qdup();
  StackDump(M.pstack, 8, "0 ?dup");
  //
  INIT_STK
  PushP = 1;
  _qdup();
  StackDump(M.pstack, 8, "1 ?dup");  
  //
  INIT_STK
  _onep();
  StackDump(M.pstack, 8, "1+");
  //
  INIT_STK
  _onen();
  StackDump(M.pstack, 8, "1-");
  //
  INIT_STK
  _fourp();
  StackDump(M.pstack, 8, "4+");
  //
  INIT_STK
  _fourn();
  StackDump(M.pstack, 8, "4-");  
  //
  INIT_STK
  _add();
  StackDump(M.pstack, 8, "add");   
  //
  INIT_STK
  _sub();
  StackDump(M.pstack, 8, "sub");   
  //
  INIT_STK
  _mul();
  StackDump(M.pstack, 8, "mul"); 
  //
  INIT_STK
  _smod();
  StackDump(M.pstack, 8, "/mod");   
  //
  INIT_STK
  _equal();
  StackDump(M.pstack, 8, "="); 
  //
  //INIT_STK
  _equal();
  StackDump(M.pstack, 8, "="); 
  //
  INIT_STK
  _nequal();
  StackDump(M.pstack, 8, "<>"); 
  //
  //INIT_STK
  _nequal();
  StackDump(M.pstack, 8, "<>");
  //
  INIT_STK
  _less();
  StackDump(M.pstack, 8, "<"); 
  //
  //INIT_STK
  _less();
  StackDump(M.pstack, 8, "<");  
  //
  INIT_STK
  _greatr();
  StackDump(M.pstack, 8, ">"); 
  //
  //INIT_STK
  _greatr();
  StackDump(M.pstack, 8, ">");   
  //
  INIT_STK
  _lesseq();
  StackDump(M.pstack, 8, "<="); 
  //
  INIT_STK
  _greatreq();
  StackDump(M.pstack, 8, ">=");
  //
  INIT_STK
  PushP = 0;
  _zeroeq();
  StackDump(M.pstack, 8, "0=");  
  //
  INIT_STK
  PushP = 1;
  _zeroeq();
  StackDump(M.pstack, 8, "0="); 
  //
  INIT_STK
  PushP = 0;
  _zeroneq();
  StackDump(M.pstack, 8, "0<>");  
  //
  INIT_STK
  PushP = 1;
  _zeroneq();
  StackDump(M.pstack, 8, "0<>");  
  //
  INIT_STK
  PushP = 1;
  _zerolth();
  StackDump(M.pstack, 8, "0<");  
  //
  INIT_STK
  PushP = 1;
  _zerogth();
  StackDump(M.pstack, 8, "0>");   
 //
  INIT_STK
  PushP = 1;
  _zltheq();
  StackDump(M.pstack, 8, "0<=");  
  //
  INIT_STK
  PushP = 1;
  _zgtheq();
  StackDump(M.pstack, 8, "0>=");  
  //
  INIT_STK
  PushP = 0xFF00;
  PushP = 0xAAAA;
  _bitsand();
  StackDump(M.pstack, 8, "AND");  
  //
  INIT_STK
  PushP = 0xFF00;
  PushP = 0xAAAA;
  _bitsor();
  StackDump(M.pstack, 8, "OR"); 
  //
  INIT_STK
  PushP = 0xFF00;
  PushP = 0xAAAA;
  _bitsxor();
  StackDump(M.pstack, 8, "XOR"); 
  //
  INIT_STK
  PushP = 0xAAAA;
  _bitsnot();
  StackDump(M.pstack, 8, "INVERT");   
}
*/
