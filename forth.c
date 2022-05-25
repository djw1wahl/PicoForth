#include "Common.h"
#include "Utilities.h"
#include "StackPrimitives.h"
#include "VFM_Build_Primitives.h"
//
MemoryImage M;
//
void forth(void){
  ClearMemory();
  //TestStkOps();
  BuildCodeEntries();
  //Dump(0,16);
  PrintDictEntries(M.HERE =0, 36);
}
