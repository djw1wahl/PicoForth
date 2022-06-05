#include "Common.h"
#include "StackPrimitives.h"
#include "SystemPrimitives.h"
#include "VFM_Build_Primitives.h"
//
int32_t  iA32, *pA32;
int8_t   *pA8, *pB8;
//
void _execute(void){ }
//
void _fnext(void)  { Notify("This is _fnext\n.") }
//
void _fexit(void)  { Notify("This is _fexit.\n") }  
//
void _docolon(void){ Notify("This is _docolon.\n") }
//  
void _docode (void){ Notify("This is _docode.\n") }  
//
char wordname[32];
void TRY_Code_Word(char *str){                      // lookup and execute a code word by its string name 
DictEntry *entry;
//      
  strcpy(wordname, str); PushP = (int32_t)wordname; // put a word name on the stack
  do{
    _find();                                          // get the Head address of the word to execute, or 0 if  not found
    if(TopP == 0){ sprintf(buf,"Can't find that word: %s\n", wordname); PrintBuf(buf); return; } // make sure word name is found 
    M.IP = PopP;                                      // Head index of found word at M.data32[idx] is stored at WP
    entry = (DictEntry*) &M.data32[M.IP];             // entry struct now pointing to the word to execute, pfa points beyond NAME[] where first location is the cfa
    ((void(*)(void)) M.data32[(entry->pfa)])();   // call the function(s), they will start with docolon() or docode() or literal() or variable(),...
  }while(true);  
}
//
void TRY_Colon_Word(char *str){                     // lookup and execute a colon word by its string name 
DictEntry *entry;
//      
  strcpy(wordname, str); PushP = (int32_t)wordname; // put a word name on the stack
  do{
    _find();                                          // get the Head address of the word to execute, or 0 if  not found
    if(TopP == 0){ sprintf(buf,"Can't find that word: %s\n", wordname); PrintBuf(buf); return; } // make sure word name is found 
    M.WP = PopP;                                      // Head index of found word at M.data32[idx] is stored at WP
    entry = (DictEntry*) &M.data32[M.WP];             // entry struct now pointing to the word to execute, pfa points beyond NAME[] where first location is the cfa
    M.WP = entry->pfa;
    PushR = M.WP +1;
  }while(true);
}
//
void _store(void)        { pA32 = (int32_t*)PopP; *pA32  =  PopP;  }                            // ( x addr -- )       store x at addr
void _fetch(void)        { pA32 = (int32_t*)PopP;  PushP   = *pA32;  }                          // ( a-addr -- x )     read value stored at addr. 
void _addstore(void)     { pA32 = (int32_t*)PopP;  iA32  = *pA32 + PopP; *pA32 = iA32; }  // ( n | u a-addr -- ) add n to value at addr.
void _substore(void)     { pA32 = (int32_t*)PopP;  iA32  = *pA32 - PopP; *pA32 = iA32; }  // ( n | u a-addr -- ) sub n to value at addr.
void _storebyte(void)    { pA8  = (int8_t*) PopP; *pA8   =  PopP;  }                            // ( c addr -- )       store c at addr
void _fetchbyte(void)    { pA8  = (int8_t*) PopP;  PushP   = *pA8; }                            // ( a-addr -- c )     read  cvalue stored at addr. 
void _ccopy(void)        { pA8  = (int8_t*) PopP;  pB8   = (int8_t*) PopP; // get both c addr
                          *pB8  = *pA8;                                    // copy
                          PushP   = (int32_t)++pB8;      // incr addrs and put back addr on stack
                          PushP   = (int32_t)++pA8; }    // ( dest_adr src_adr - dest_adr+1 src_adr+1 ) copy c ( dest <- src )
void _cmove(void)        { iA32 = PopP;                  // get the count from the stack
                           pA8  = (int8_t*) PopP;  
                           pB8  = (int8_t*) PopP;        // ARE THE DST & SRC ADR BACKWARDS ???
                           memcpy(pB8, pA8, iA32); } // do the block copy                     // (  dest_adr src_adr n - )
//                      
void _key(void)          { PushP = serial_getchar(); }     // one char at a time is put on stack, this blocks waiting for a char.
void _emit(void)         { serial_putchar((char)PopP); }   // send one char to the UART
// 
void _number(void)       { // NOTE: unlike the ANS Forth this forth uses C strings which are null terminated
                           // ( string -- intnum (error == false: means passed) )
                           int32_t accum = 0;
                           int32_t sign  = 1;               // if '-' is prefix then this is set to -1
                           int32_t i=0, j;
                           bool match = false;
                           char*   str  = (char*)PopP;     // get the string adr
                           int32_t len  = strlen(str);     // get the str length
                           switch(str[i]){
                            case '-': sign = -1; str++; i=0; len--; break;
                            case '+':            str++; i=0; len--; break;
                           }                           
                           // need to test following chars to see if they are digits in the current BASE
                           const char *charlowlist  = "0123456789abcdefghijklmnopqrstuvwxyz";
                           const char *charhighlist = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                           do{
                             for(j=0;j<M.BASE;j++){
                              if( str[i] == charlowlist[j]) { match = true; break; }
                              else{
                               if(str[i] == charhighlist[j]){ match = true; break; } 
                              }               
                             }       
                             if(match){ // do something smart
                              accum *= M.BASE;  accum += j;                           
                             }else { // failure 
                               PushP = accum; PushP = true; return; // pushing true indicates failure
                             }
                           }while (++i<len);         
                           // successful conversion
                           accum *= sign;
                           PushP = accum; PushP = false; // pushing false indicates success
}
#ifdef TEST_NUMBER
  void testnumber(void){
    static char* astring = "0110111110100101";
    PushP = (int32_t) astring;
    M.BASE = 2;
    _number();
    bool result = PopP;
    int32_t num = PopP;
    sprintf(buf,"NUMBER = %X %d\n",num, result);
    PrintBuf(buf);
  }
#endif
//
void _find(void){ // NOTE: unlike the ANS Forth this forth uses C strings which are null terminated
                  // ( string -- dict_adr_of_head | zero_if_not_found) )
                  //
                  char*   str  = (char*)PopP;     // get the string adr
                  int32_t latest = M.LATEST;
                  do{
                      DictEntry *start = (DictEntry*) &M.data32[latest] ;
                      if(strcmp(start->Name, str) == 0){ // found                        
                        if(start->Hidden){ // do not FIND Hidden entries
                          PushP = 0; // not found
                          return;
                        }else{ // Found 
                          PushP = latest;
                          return; // this is where a successful find exits
                        }
                      }
                      latest = start->Link;             
                  }while(latest > 0);     
                  PushP = 0; // not found             
}
#ifdef TEST_FIND
  void testfind(char* astring){
    DictEntry* entry;
    PushP = (int32_t) astring;
    _find();
    int32_t idx = PopP;
    entry = (DictEntry*) &M.data32[idx];
    if(idx == 0){ 
      sprintf(buf,"%s Not Found.\n", astring); PrintBuf(buf);
      return;
    }
    sprintf(buf,"Find Found = %s\n",entry->Name);
    PrintBuf(buf);
  }
#endif
void _tcfa(void){ 
                    DictEntry*    entry = (DictEntry*) PopP;
                    PushP = (int32_t) M.data32[entry->pfa]; // now TopP contains (void)(*)(void) of codetype for this word
}
void _tdfa(void){ 
                    DictEntry*    entry = (DictEntry*) PopP;
                    PushP = (int32_t) M.data32[(entry->pfa)+4]; // now TopP contains first parameter word, agnostic type
}
void _interpret(void){ 
  
}
void _create(void){ }
void _comma(void){ }
void _lbrac(void){ }
void _rbrac(void){ }
void _colon(void){ }
void _semicolon(void){ }
void _immediate(void){ }
void _hidden(void){ }
void _hide(void){ }
void _tick(void){ }
void _branch(void){ }
void _zbranch(void){ }
void _litstring(void){ }
void _tell(void){ }
void _quit(void){ }
void _char(void){ }

