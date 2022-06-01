#include "Common.h"
#include "Utilities.h"
#include "StackPrimitives.h"
#include "VFM_Build_Primitives.h"
#include "SystemPrimitives.h"
//
MemoryImage M;
//
void forth(void){
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
