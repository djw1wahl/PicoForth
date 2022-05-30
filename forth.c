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
  //TestStkOps();
  BuildCodeEntries(0);
  //Dump(0,0xC0);
  //PrintDictEntries(128); 
  //testnumber();
  //testfind("0=");
}
