#include "Common.h"
#include "Utilities.h"
#include "StackPrimitives.h"
#include "VFM_Build_Primitives.h"
#include "SystemPrimitives.h"
//
MemoryImage M;
//
#ifndef LINUX
void forth(void){
#endif
#ifdef LINUX
void main(){
#endif
  ClearMemory();
  BuildCodeEntries(DICTINARY_START);
  //
#ifdef RUN_STACK_TESTS
  TestStkOps();
#endif
#ifdef PRINT_DICT_ENTRIES
  //Dump(DICTINARY_START);
  PrintDictEntries();
#endif 
#ifdef TEST_NUMBER
  testnumber();
#endif
#ifdef TEST_FIND
  testfind("DUP");
#endif
}
#ifdef LINUX
  void PrintBuf(char* buf){ for(int32_t i=0;i<strlen(buf);i++){ putchar(buf[i]); } }
  void PrintNumber(int32_t n){ char abuf[16]; sprintf(abuf," %d", n); PrintBuf(abuf); }
#endif
