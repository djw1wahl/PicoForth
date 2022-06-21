#include "Common.h"
#include "StackPrimitives.h"
#include "SystemPrimitives.h"
#include "VFM_Build_Primitives.h"
#include "VFM.h"
//
//--------------------------HERE BE DRAGONS------------------------------------------------------------------------------
#define _HEADINDEX2CFA_(index) ((DictEntry*)   &M.data32[index]->cfa)
#define _CFA2DICTENTRYPTR_     (&((DictEntry*)M.W)->cfa)-8)
#define _TRAVERSE_             ((void(*)(void)) M.data32[ M.PC ]) ()
//
void Show(char* tell){
                               Notify("\n");  
                               Notify(tell); 
                               PrintNumber( TopR );
                               PrintNumber( M.PC );
                               Notify("\n");  
}
//
void _next        (void)    { 
                               M.PC = M.data32[PopR +1];
                               Show("Next   ");
                               do{}while(1);
                               if(M.PC==0)return;         
}
//
void _docolon     (void)    {  
                               PushR = (++M.PC);     
                               M.PC = M.data32[TopR];                                             
                               Show("DoColon");
                               _TRAVERSE_ ;
}
//
void _docode      (void)    {                              
                               //M.PC = 0;      
                               //do{ ((void(*)(void)) M.data32[ _CFA_ + (++M.PC)]) (); }while(M.PC != 0);
}
//   
void _doliteral   (void)    { }
//
void _doconstant  (void)    { }
//
void _dovariable  (void)    { }
//
void _rtn         (void)    { Show("RTN\n"); }
//
void DemoUsingVFM(char* demo_this_word){  // parameter is NameOfWordToExecute
  PushP = (int32_t) demo_this_word;       // push a pointer to the word name on the Pstack
  _find();                                // run find
  M.I = PopP;                             // get result of 'find' which is Index of Word Head
  if(M.I == 0){ sprintf(buf,"%s Not Found.\n", demo_this_word); PrintBuf(buf);  return; }
  //
  Notify("\n<Ready to Interpret>");               // M.I is the index into the M.Data32 space, identifying the word to interpret
  M.W   = (int32_t)((DictEntry*) &M.data32[M.I]); // M.W now is (DictEntry*) Target Word  !NOT updated thru _TRAVERSE_!
  M.PC = ((DictEntry*)M.W)->cfa;                  // M.PC now is cfa( Target Word ) 
                                         
  //PrintNumber((int32_t)  &((DictEntry*)M.W)->Link);
  //PrintNumber((int32_t) (&((DictEntry*)M.W)->cfa)-8);
                                                                      
  Show("VFM    ");
  _TRAVERSE_ ;                            // Run the code at the M.PC  
  //
  Notify("\n--Success--\n");
}  
//--------------------------DRAGONS ABOVE--------------------------------------------------------------------------------
