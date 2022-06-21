#include "Common.h"
#include "Utilities.h"
#include "StackPrimitives.h"
#include "VFM_Build_Primitives.h"
#include "VFM.h"
#include "SystemPrimitives.h"
//
MemoryImage M;
//
void forth(void){
  ClearMemory();
  BuildCodeEntries(DICTINARY_START);
  BuildWordEntries(M.HERE); 
  //
#ifdef RUN_STACK_TESTS
  TestStkOps();
#endif
#ifdef PRINT_DICT_ENTRIES
  Dump(0x0254);
  //Dump(DICTINARY_START);
  //PrintDictEntries("WORD_TEST", "FIND");
#endif 
#ifdef TEST_NUMBER
  testnumber();
#endif
#ifdef TEST_FIND
  testfind("DUP");
#endif
  //
  DemoUsingVFM("WORD_TEST");
  //
}
