#include "Common.h"
#include "StackPrimitives.h"
#include "VFM_Build_Primitives.h"
//
void ClearMemory(void){ 
int i;  
  for(i=0;i<MEMSIZE;i++)  { M.data32[i] = 0; } 
  for(i=0;i<STACKSIZE;i++){ M.rstack[i] = 0; }
  for(i=0;i<STACKSIZE;i++){ M.pstack[i] = 0; }
  for(i=0;i<STACKSIZE;i++){ M.fstack[i] = 0.0; }  
  M.STATE=0;
  M.LATEST=0;
  M.HERE=0;
  M.BASE=0;  
  M.IP     =0;
  M.ridx   =0;
  M.pidx   =0;
  M.fidx   =0;
}
//
//
//                ( L5thP,         L4thP,         L3rdP,        L2ndP, 
//                  TopP, 
//                  H2ndP,         H3rdP,         H4thP,        H5thP )
//
#ifdef RUN_STACK_TESTS
char abuf[128];
char ebuf[128];
char sbuf[128];
void run_op(int32_t nstk5, int32_t nstk4, int32_t nstk3, int32_t nstk2, \
             int32_t nstkT, \
             int32_t pstk2, int32_t pstk3, int32_t pstk4, int32_t pstk5, \
             void (*fptr)(void)){  // this tester sets up the stack, runs the test code, 
                                   // and compares the expected stack result
                     //
                     ResetStackP
                     //
                     L5thP = nstk5; L4thP = nstk4; L3rdP = nstk3; L2ndP = nstk2; 
                     TopP  = nstkT;
                     H2ndP = pstk2; H3rdP = pstk3; H4thP = pstk4; H5thP = pstk5; 
             fptr(); // execute the code under test
             //
             sprintf(sbuf," %6d,   %6d,   %6d,   %6d,   %6d,   %6d,   %6d,   %6d,  %6d",\
                            nstk5, nstk4, nstk3, nstk2, nstkT, pstk2, pstk3, pstk4, pstk5);
             //
             sprintf(abuf," %6d,   %6d,   %6d,   %6d,   %6d,   %6d,   %6d,   %6d,  %6d",\
                            L5thP, L4thP, L3rdP, L2ndP, TopP, H2ndP, H3rdP, H4thP, H5thP);          
}
//
void  run_cmp(int32_t nstk5, int32_t nstk4, int32_t nstk3, int32_t nstk2, \
              int32_t nstkT, \
              int32_t pstk2, int32_t pstk3, int32_t pstk4, int32_t pstk5, \
              char* testname ){
             //
             char mess[40];
             sprintf(ebuf," %6d,   %6d,   %6d,   %6d,   %6d,   %6d,   %6d,   %6d,  %6d",\
                            nstk5, nstk4, nstk3, nstk2, nstkT, pstk2, pstk3, pstk4, pstk5);
             if(strcmp(ebuf, abuf) == 0){ // test passes
               sprintf(mess,"Pass: [%s]\n", testname);
               PrintBuf(mess);
             }else {
               sprintf(mess,"Fail: [%s]\n", testname);
               PrintBuf(mess);
               PrintBuf(sbuf); PrintBuf("<- Stimulus\n");
               PrintBuf(ebuf); PrintBuf("<- Expected\n");
               PrintBuf(abuf); PrintBuf("<- Actual\n");              
             }
}
//
void TestStkOps(void){
  //                   ^
  run_op ( 0, 0, 0, 0, 1, 0, 0, 0, 0, _dup);
  run_cmp( 0, 0, 0, 1, 1, 0, 0, 0, 0, "DUP");
  //                   ^
  run_op ( 0, 0, 0, 2, 1, 0, 0, 0, 0, _swap);
  run_cmp( 0, 0, 0, 1, 2, 0, 0, 0, 0, "SWAP");  
  //                   ^
  run_op ( 0, 0, 0, 2, 1, 0, 0, 0, 0, _drop);
  run_cmp( 0, 0, 0, 0, 2, 0, 0, 0, 0, "DROP");
  //                   ^
  run_op ( 0, 0, 1, 2, 3, 0, 0, 0, 0, _over);
  run_cmp( 0, 1, 2, 3, 2, 0, 0, 0, 0, "OVER");
  //                   ^
  run_op ( 0, 0, 3, 2, 1, 0, 0, 0, 0, _prot);
  run_cmp( 0, 0, 2, 1, 3, 0, 0, 0, 0, "ROT");
  //                   ^  
  run_op ( 0, 0, 3, 2, 1, 0, 0, 0, 0, _nrot);
  run_cmp( 0, 0, 1, 3, 2, 0, 0, 0, 0, "-ROT");
  //                   ^  
  run_op ( 0, 0, 0, 2, 1, 0, 0, 0, 0, _2dup);
  run_cmp( 0, 2, 1, 2, 1, 0, 0, 0, 0, "2DUP");
  //                   ^    
  run_op ( 0, 4, 3, 2, 1, 0, 0, 0, 0, _2swap);
  run_cmp( 0, 2, 1, 4, 3, 0, 0, 0, 0, "2SWAP");
  //                   ^ 
  run_op ( 0, 0, 0, 1, 0, 0, 0, 0, 0, _qdup);
  run_cmp( 0, 0, 0, 1, 0, 0, 0, 0, 0, "0 QDUP");
  //                   ^
  run_op ( 0, 0, 0, 0, 1, 0, 0, 0, 0, _qdup);
  run_cmp( 0, 0, 0, 1, 1, 0, 0, 0, 0, "1 QDUP");  
  //                   ^   
  run_op ( 0, 0, 0, 0, 1, 0, 0, 0, 0, _onep);
  run_cmp( 0, 0, 0, 0, 2, 0, 0, 0, 0, "1+");   
  //                   ^   
  run_op ( 0, 0, 0, 0, 2, 0, 0, 0, 0, _onen);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "1-");   
  //                   ^   
  run_op ( 0, 0, 0, 0, 1, 0, 0, 0, 0, _fourp);
  run_cmp( 0, 0, 0, 0, 5, 0, 0, 0, 0, "4+");   
  //                   ^   
  run_op ( 0, 0, 0, 0, 5, 0, 0, 0, 0, _fourn);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "4-"); 
  //                   ^   
  run_op ( 0, 0, 0, 4, 5, 0, 0, 0, 0, _add);
  run_cmp( 0, 0, 0, 0, 9, 5, 0, 0, 0, "+"); 
  //                   ^   
  run_op ( 0, 0, 0, 5, 4, 0, 0, 0, 0, _sub);
  run_cmp( 0, 0, 0, 0, 1, 4, 0, 0, 0, "-"); 
  //                   ^   
  run_op ( 0, 0, 0, 2, 3, 0, 0, 0, 0, _mul);
  run_cmp( 0, 0, 0, 0, 6, 3, 0, 0, 0, "*"); 
  //
  run_op ( 0, 0, 0, 6, 2, 0, 0, 0, 0, _div);
  run_cmp( 0, 0, 0, 0, 3, 2, 0, 0, 0, "/"); 
  //                   ^         
  run_op ( 0, 0, 0, 16,4, 0, 0, 0, 0, _smod);
  run_cmp( 0, 0, 0, 0, 0, 4, 0, 0, 0, "/MOD"); 
  //                   ^   
  run_op ( 0, 0, 0, 17,4, 0, 0, 0, 0, _smod);
  run_cmp( 0, 0, 0, 0, 1, 4, 0, 0, 0, "/MOD");  
  //                   ^   
  run_op ( 0, 0, 0, 3, 3, 0, 0, 0, 0, _equal);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "=");
  //                   ^   
  run_op ( 0, 0, 0, 3, 4, 0, 0, 0, 0, _equal);
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "=");    
  //                   ^   
  run_op ( 0, 0, 0, 3, 3, 0, 0, 0, 0, _nequal);
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "<>");
  //                   ^   
  run_op ( 0, 0, 0, 3, 4, 0, 0, 0, 0, _nequal);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "<>");   
  //                   ^   
  run_op ( 0, 0, 0, 3, 3, 0, 0, 0, 0, _less); 
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "<");
  //                   ^   
  run_op ( 0, 0, 0, 3, 4, 0, 0, 0, 0, _less);  
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "<");  
  //                   ^   
  run_op ( 0, 0, 0, 4, 3, 0, 0, 0, 0, _less);
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "<");    
  //                   ^   
  run_op ( 0, 0, 0, 3, 3, 0, 0, 0, 0, _greatr); 
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, ">");
  //                   ^   
  run_op ( 0, 0, 0, 3, 4, 0, 0, 0, 0, _greatr);  
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, ">");  
  //                   ^   
  run_op ( 0, 0, 0, 4, 3, 0, 0, 0, 0, _greatr);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, ">");   
  //                   ^   
  run_op ( 0, 0, 0, 3, 3, 0, 0, 0, 0, _lesseq); 
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "<=");
  //                   ^   
  run_op ( 0, 0, 0, 3, 4, 0, 0, 0, 0, _lesseq);  
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "<=");  
  //                   ^   
  run_op ( 0, 0, 0, 4, 3, 0, 0, 0, 0, _lesseq);
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "<=");  
  //                   ^   
  run_op ( 0, 0, 0, 3, 3, 0, 0, 0, 0, _greatreq); 
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, ">=");
  //                   ^   
  run_op ( 0, 0, 0, 3, 4, 0, 0, 0, 0, _greatreq);  
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, ">=");  
  //                   ^   
  run_op ( 0, 0, 0, 4, 3, 0, 0, 0, 0, _greatreq);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, ">=");   
  //                   ^   
  run_op ( 0, 0, 0, 0, 4, 0, 0, 0, 0, _zeroeq);  
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "0=");  
  //                   ^   
  run_op ( 0, 0, 0, 0, 0, 0, 0, 0, 0, _zeroeq);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "0=");   
  //                   ^   
  run_op ( 0, 0, 0, 0, 4, 0, 0, 0, 0, _zeroneq);  
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "0<>");  
  //                   ^   
  run_op ( 0, 0, 0, 0, 0, 0, 0, 0, 0, _zeroneq);
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "0<>");   
  //                   ^   
  run_op ( 0, 0, 0, 0, -1, 0, 0, 0, 0, _zerolth);  
  run_cmp( 0, 0, 0, 0,  1, 0, 0, 0, 0, "0<");  
  //                   ^   
  run_op ( 0, 0, 0, 0, 1, 0, 0, 0, 0, _zerolth);
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "0<");     
  //                   ^   
  run_op ( 0, 0, 0, 0, 1, 0, 0, 0, 0, _zltheq);  
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "0<=");  
  //                   ^   
  run_op ( 0, 0, 0, 0, 0, 0, 0, 0, 0, _zltheq);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "0<=");
  //                   ^   
  run_op ( 0, 0, 0, 0, -1, 0, 0, 0, 0, _zltheq);
  run_cmp( 0, 0, 0, 0,  1, 0, 0, 0, 0, "0<=");  
  //                   ^   
  run_op ( 0, 0, 0, 0, 1, 0, 0, 0, 0, _zgtheq);  
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "0>=");  
  //                   ^   
  run_op ( 0, 0, 0, 0, 0, 0, 0, 0, 0, _zgtheq);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "0>=");  
  //                   ^   
  run_op ( 0, 0, 0, 0, -1, 0, 0, 0, 0, _zgtheq);
  run_cmp( 0, 0, 0, 0,  0, 0, 0, 0, 0, "0>=");  
  //                   ^   
  run_op ( 0, 0, 0, 2, 1, 0, 0, 0, 0, _bitsand);
  run_cmp( 0, 0, 0, 0, 0, 1, 0, 0, 0, "AND");   
  //                   ^   
  run_op ( 0, 0, 0, 1, 1, 0, 0, 0, 0, _bitsand);
  run_cmp( 0, 0, 0, 0, 1, 1, 0, 0, 0, "AND");   
  //                   ^   
  run_op ( 0, 0, 0, 2, 1, 0, 0, 0, 0, _bitsor);
  run_cmp( 0, 0, 0, 0, 3, 1, 0, 0, 0, "OR");   
  //                   ^   
  run_op ( 0, 0, 0, 1, 1, 0, 0, 0, 0, _bitsxor);
  run_cmp( 0, 0, 0, 0, 0, 1, 0, 0, 0, "XOR");    
  //                   ^   
  run_op ( 0, 0, 0, 0, 0, 0, 0, 0, 0, _bitsxor);
  run_cmp( 0, 0, 0, 0, 0, 0, 0, 0, 0, "XOR"); 
  //                   ^   
  run_op ( 0, 0, 0, 0, 1, 0, 0, 0, 0, _bitsxor);
  run_cmp( 0, 0, 0, 0, 1, 1, 0, 0, 0, "XOR");    
  //                   ^   
  run_op ( 0, 0, 0, 1, 0, 0, 0, 0, 0, _bitsxor);
  run_cmp( 0, 0, 0, 0, 1, 0, 0, 0, 0, "XOR");   
  //                   ^   
  run_op ( 0, 0, 0, 0, 0, 0, 0, 0, 0, _bitsnot);
  run_cmp( 0, 0, 0, 0, -1, 0, 0, 0, 0, "INVERT"); 
}
#endif
