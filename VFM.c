#include "Common.h"
#include "StackPrimitives.h"
#include "SystemPrimitives.h"
#include "VFM_Build_Primitives.h"
#include "VFM.h"
//
//--------------------------HERE BE DRAGONS------------------------------------------------------------------------------
#define HEADINDEX2PTR(index) ((DictEntry*) &M.data32[index])
#define aPC ((DictEntry*)M.W)->PC
//
enum DoThis{ next, docolon, docode, doliteral, doconstant, dovariable, rtn }op;
//
void _next        (void)    { op = next;        }
void _docolon     (void)    { op = docolon;     }
void _docode      (void)    { op = docode;      }   
void _doliteral   (void)    { op = doliteral;   }
void _doconstant  (void)    { op = doconstant;  }
void _dovariable  (void)    { op = dovariable;  }
void _rtn         (void)    { op = rtn;         }
//
void _interpret(void){ 
  ((void(*)(void)) M.data32[((DictEntry*)M.W)->cfa]) (); // call the (cfa)() of this word
  //                                                     // often the call will be to docolon, so op = docolon.
  aPC = 0;                                               // This is the program counter which traverses the pfalist.
  //
                       Notify("\nIntrp: "); 
                       Notify(((DictEntry*)M.W)->Name);  
Interpret:
  switch(op){
    case next:       { 
                       M.W = PopR;            
                       if(M.W == 0)goto Exit_Interpret; // is this correct way to exit?                                
                       _interpret(); // Dragon Fire                                                                                                 
                     } break;
    case docolon:    {                       
                       if(((DictEntry*) M.W)->NumParam > (aPC)+2){
                         PushR = M.data32[((DictEntry*) M.W)->cfa+(aPC)+2]; // pushing pfa[1,3,5,...]                                            
                         Notify("\n Push: "); 
                         Notify(((DictEntry*)TopR)->Name); 
                       }          

                       M.W = M.data32[((DictEntry*) M.W)->cfa+(aPC)+1];
                      
                       Notify("\n Post: "); 
                       Notify(((DictEntry*)M.W)->Name); 
                       
                       aPC += 2;
                       _interpret(); // Dragon Fire 
                       aPC  = 0;
                       //       
                     } break;
    case docode:     { 
                       int32_t i=0;
                       do{ // this calls the primitives in sequence until _rtn() which changes <op> to next. 
                           ((void(*)(void)) M.data32[((DictEntry*) M.W)->cfa + (++i)]) ();        
                       }while(op == docode);
                     } break;
    case doliteral:  { 
                     } break;
    case doconstant: { 
                     } break;
    case dovariable: { 
                     } break;
    case rtn:        { op = next; } break;
  }
  goto Interpret;
Exit_Interpret: return;
}
//
void DemoUsingVFM(char* demo_this_word){  // parameter is NameOfWordToExecute
  PushP = (int32_t) demo_this_word;       // push a pointer to the word name on the Pstack
  _find();                                // run find
  M.I = PopP;                             // get result of 'find' which is Index of Word Head
  if(M.I == 0){ sprintf(buf,"%s Not Found.\n", demo_this_word); PrintBuf(buf);  return; }
  //
  Notify("\n<Ready to Interpret>"); 
                            // M.I is the index into the M.Data32 space, identifying the word to interpret
  M.W = (int32_t)HEADINDEX2PTR(M.I);      // M.W now is DictEntry Pointer to Target Word  
  _interpret();                           
  Notify("\n--Success--\n");
}  
//--------------------------DRAGONS ABOVE--------------------------------------------------------------------------------
