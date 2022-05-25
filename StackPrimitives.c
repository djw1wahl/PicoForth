#include "Common.h"
#include "StackPrimitives.h"
//
// using underscore to prefix the code primitives avoids collision with some builtin C functions like min, exit, quit,...
//
void _dup (void)   { PushP = TopP; }                                                                               // ( a b c - a b c c )
void _drop(void)   { DecP }                                                                                        // ( a b c - a b )
void _swap(void)   { M.TMPA = TopP; TopP = M.pstack[M.pidx -1]; M.pstack[M.pidx -1] = M.TMPA; }                    // ( a b c - a c b )
void _over(void)   { M.TMPA = M.pstack[M.pidx -1]; PushP = M.TMPA; }                                               // ( a b c - a b c b )
void _prot(void)   { M.TMPC = M.pstack[M.pidx -0]; M.TMPB = M.pstack[M.pidx -1]; M.TMPA = M.pstack[M.pidx -2]; \
                     M.pstack[M.pidx -0] = M.TMPA; M.pstack[M.pidx -1] = M.TMPC; M.pstack[M.pidx -2] = M.TMPB; }   // ( a b c - b c a ) 
void _nrot(void)   { M.TMPC = M.pstack[M.pidx -0]; M.TMPB = M.pstack[M.pidx -1]; M.TMPA = M.pstack[M.pidx -2]; \
                     M.pstack[M.pidx -0] = M.TMPB; M.pstack[M.pidx -1] = M.TMPA; M.pstack[M.pidx -2] = M.TMPC; }   // ( a b c - c a b )
void _2drop(void)  { M.pidx -= 2; }                                                                                // ( a b c - a )
void _2dup(void)   { M.pidx += 2; \
                     M.pstack[M.pidx] = M.pstack[M.pidx -2]; M.pstack[M.pidx -1] = M.pstack[M.pidx -3]; }          // ( a b c - a b c b c )
void _2swap(void)  { M.TMPA = M.pstack[M.pidx -3]; M.TMPB = M.pstack[M.pidx -2]; \
                     M.pstack[M.pidx -3] = M.pstack[M.pidx -1]; M.pstack[M.pidx -2] = M.pstack[M.pidx -0]; \
                     M.pstack[M.pidx -1] = M.TMPA; M.pstack[M.pidx -0] = M.TMPB; }                                 // ( a b c d - c d a b )
void _qdup(void)   { if(TopP){ PushP = TopP; } }                                                                   // ( x - 0 | x x )     
void _onep(void)   { M.pstack[M.pidx]++; }                                                                         // ( x - x+1 )            
void _onen(void)   { M.pstack[M.pidx]--; }                                                                         // ( x - x-1 )
void _fourp(void)  { M.pstack[M.pidx]+= 4; }                                                                       // ( x - x+4 )            
void _fourn(void)  { M.pstack[M.pidx]-= 4; }                                                                       // ( x - x-4 )
void _add(void)    { M.pstack[M.pidx-1] += TopP; DecP }                                                            // ( x y - x+y )
void _sub(void)    { M.pstack[M.pidx-1] -= TopP; DecP }                                                       // ( x y - x-y )
void _mul(void)    { M.pstack[M.pidx-1] *= TopP; DecP }                                                       // ( x y - x*y )
void _smod(void)   { M.TMPA = M.pstack[M.pidx-1] % TopP; \
                     M.TMPB = M.pstack[M.pidx-1] / TopP; \
                     M.pidx -= 2; \
                     PushP = M.TMPA; PushP = M.TMPB; }                                                             // ( x y - x%y x/y )
void _equal(void)   { if(M.pstack[M.pidx-1] == M.pstack[M.pidx]) \
                         { DecP PushP = true; } \
                     else{ DecP PushP = false; } }                                                            // ( x y - x==y | x!=y )    
void _nequal(void)  { if(M.pstack[M.pidx-1] != M.pstack[M.pidx]) \
                         { DecP PushP = true; } \
                     else{ DecP PushP = false; } }                                                            // ( x y - x!=y | x==y ) 
void _less(void)    { if(M.pstack[M.pidx-1] < M.pstack[M.pidx]) \
                         { DecP PushP = true; } \
                     else{ DecP PushP = false; } }                                                            // ( x y - x<y | x!<y ) 
void _greatr(void)  { if(M.pstack[M.pidx-1] > M.pstack[M.pidx]) \
                         { DecP PushP = true; } \
                     else{ DecP PushP = false; } }                                                            // ( x y - x>y | x!>y )    
void _lesseq(void)  { if(M.pstack[M.pidx-1] <= M.pstack[M.pidx]) \
                         { DecP PushP = true; } \
                     else{ DecP PushP = false; } }                                                            // ( x y - x<=y | x!<=y ) 
void _greatreq(void){ if(M.pstack[M.pidx-1] >= M.pstack[M.pidx]) \
                         { DecP PushP = true; } \
                     else{ DecP PushP = false; } }                                                            // ( x y - x>=y | x!>=y ) 
void _zeroeq(void)  { if(0 == M.pstack[M.pidx--]) \
                         { PushP = true; } \
                     else{ PushP = false; } }                                                                 // ( x - 0= | !0= )
void _zeroneq(void) { if(0 != M.pstack[M.pidx--]) \
                         { PushP = true; } \
                     else{ PushP = false; } }                                                                 // ( x - 0<> | !0<> )
void _zerolth(void) { if( 0< M.pstack[M.pidx--]) \
                         { PushP = true; } \
                     else{ PushP = false; } }                                                                 // ( x - 0< | !0< ) 
void _zerogth(void) { if( 0> M.pstack[M.pidx--]) \
                         { PushP = true; } \
                     else{ PushP = false; } }                                                                 // ( x - 0> | !0> )   
void _zltheq(void)  { if( 0<= M.pstack[M.pidx--]) \
                         { PushP = true; } \
                     else{ PushP = false; } }                                                                 // ( x - 0<= | !0<= ) 
void _zgtheq(void)  { if( 0>= M.pstack[M.pidx--]) \
                         { PushP = true; } \
                     else{ PushP = false; } }                                                                 // ( x - 0>= | !0>= )     
void _bitsand(void) { M.TMPA = M.pstack[M.pidx -1] & M.pstack[M.pidx--]; DecP PushP = M.TMPA; }               // ( x y - x&y )
void _bitsor(void)  { M.TMPA = M.pstack[M.pidx -1] | M.pstack[M.pidx--]; DecP PushP = M.TMPA; }               // ( x y - x|y )
void _bitsxor(void) { M.TMPA = M.pstack[M.pidx -1] ^ M.pstack[M.pidx--]; DecP PushP = M.TMPA; }               // ( x y - x^y )
void _bitsnot(void) { M.TMPA = ~M.pstack[M.pidx--]; PushP = M.TMPA; }                                         // ( x - !x )
//
void _fth_exit(void){ M.WP = PopR; } // Word Pointer gets top of return stack--
