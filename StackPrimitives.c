#include "Common.h"
#include "StackPrimitives.h"
//
// using underscore to prefix the code primitives avoids collision with some builtin C functions like min, exit, quit,...
// also note: some of these may be more instructions than neccesary, 
// for clarity after a drop, the stack is kept clean to the right and a 0 inserted.
// 
int32_t Tmp;
//
void _dup (void)    { PushP = TopP; }                                                                               // ( a b c - a b c c )
void _drop(void)    { TopP = 0; DecP }                                                                              // ( a b c - a b )
void _swap(void)    { Tmp = TopP; TopP = L2ndP; L2ndP = Tmp; }                                                      // ( a b c - a c b )
void _over(void)    { PushP = L2ndP; }                                                                              // ( a b c - a b c b )

void _prot(void)    { Tmp = TopP;                   // save c
                      TopP  = L3rdP;                // move a to top
                      L3rdP = L2ndP;                // move b to a 
                      L2ndP = Tmp; }                // move c to b                                                  // ( a b c - b c a ) 
void _nrot(void)    { Tmp = TopP;                   // save c
                      TopP  = L2ndP;                // move b to top
                      L2ndP = L3rdP;                // move a to b
                      L3rdP = Tmp; }                // move c to a                                                  // ( a b c - c a b )                       
void _2drop(void)   { M.pidx -= 2; }                                                                                // ( a b c - a )
void _2dup(void)    { M.pidx += 2; 
                      TopP  = L3rdP;
                      L2ndP = L4thP; }                                                                              // ( a b c - a b c b c )
void _2swap(void)   { Tmp = TopP; 
                      TopP  = L3rdP;
                      L3rdP = Tmp;
                      Tmp = L2ndP;
                      L2ndP = L4thP ;
                      L4thP = Tmp ; }                                                                               // ( a b c d - c d a b )
void _qdup(void)    { if(TopP){ PushP = TopP; } }                                                                   // ( x - 0 | x x )     
void _onep(void)    { TopP++; }                                                                                     // ( x - x+1 )            
void _onen(void)    { TopP--; }                                                                                     // ( x - x-1 )
void _fourp(void)   { TopP  += 4; }                                                                                 // ( x - x+4 )            
void _fourn(void)   { TopP  -= 4; }                                                                                 // ( x - x-4 )
void _add(void)     { L2ndP += TopP; DecP }                                                                         // ( x y - x+y )
void _sub(void)     { L2ndP -= TopP; DecP }                                                                         // ( x y - x-y )
void _mul(void)     { L2ndP *= TopP; DecP }                                                                         // ( x y - x*y )
void _div(void)     { L2ndP /= TopP; DecP }                                                                         // ( x y - x/y )
void _smod(void)    { L2ndP %= TopP; DecP }                                                                         // ( x y - x%y )
void _equal(void)   { Tmp = (L2ndP == TopP); TopP = 0; DecP; TopP = Tmp; }                                          // ( x y - x==y | x!=y )    
void _nequal(void)  { Tmp = (L2ndP != TopP); TopP = 0; DecP; TopP = Tmp; }                                          // ( x y - x!=y | x==y ) 
void _less(void)    { Tmp = (L2ndP  < TopP); TopP = 0; DecP; TopP = Tmp; }                                          // ( x y - x<y | x!<y ) 
void _greatr(void)  { Tmp = (L2ndP  > TopP); TopP = 0; DecP; TopP = Tmp; }                                          // ( x y - x>y | x!>y )    
void _lesseq(void)  { Tmp = (L2ndP <= TopP); TopP = 0; DecP; TopP = Tmp; }                                          // ( x y - x<=y | x!<=y ) 
void _greatreq(void){ Tmp = (L2ndP >= TopP); TopP = 0; DecP; TopP = Tmp; }                                          // ( x y - x>=y | x!>=y ) 
void _zeroeq(void)  { TopP = ( 0 == TopP); }                                                                        // ( x - 0= | !0= )
void _zeroneq(void) { TopP = ( 0 != TopP); }                                                                        // ( x - 0= | !0= )
void _zerolth(void) { TopP = ( TopP <  0 ); }                                                                       // ( x - 0< | !0< ) 
void _zerogth(void) { TopP = ( TopP >  0 ); }                                                                       // ( x - 0> | !0> )   
void _zltheq(void)  { TopP = ( TopP <= 0 ); }                                                                       // ( x - 0<= | !0<= ) 
void _zgtheq(void)  { TopP = ( TopP >= 0 ); }                                                                       // ( x - 0>= | !0>= )     
void _bitsand(void) { L2ndP &= TopP; DecP }                                                                         // ( x y - x&y )
void _bitsor(void)  { L2ndP |= TopP; DecP }                                                                         // ( x y - x|y )
void _bitsxor(void) { L2ndP ^= TopP; DecP }                                                                         // ( x y - x^y )
void _bitsnot(void) { TopP   = ~TopP; }                                                                             // ( x - !x )
//
void _docodetest(void){ Notify("Code Word\n")}
//
void _tor(void){ }
void _fromr(void){ }
void _rspfetch(void){ }
void _rspstore(void){ }
void _rdrop(void){ }
void _dspfetch(void){ }
void _dspstore(void){ }
