//
// stack index is limited to 0:255 which creates a circular stack
// the present stack index value points to the top of the stack
//
#define Push(x,y) (x[++y])
#define Pop(x,y)  (x[y--])
#define Top(x,y)  (x[y])
#define Nth(x,y,z)(x[y+z])
//
#define PushP Push(M.pstack,M.pidx)
// usage: PushP = value;
#define PopP  Pop (M.pstack,M.pidx)
// usage: value = PopP;
//
#define H5thP Nth(M.pstack,M.pidx,4)
#define H4thP Nth(M.pstack,M.pidx,3)
#define H3rdP Nth(M.pstack,M.pidx,2)
#define H2ndP Nth(M.pstack,M.pidx,1)
//
#define TopP  Nth (M.pstack,M.pidx,0)
// usage: value = TopP; 
//        TopP  = value;
//
#define L2ndP Nth(M.pstack,M.pidx,-1)
#define L3rdP Nth(M.pstack,M.pidx,-2)
#define L4thP Nth(M.pstack,M.pidx,-3)
#define L5thP Nth(M.pstack,M.pidx,-4)
//
#define ResetStackP M.pidx = 0;
//
// the above: HxxxP & LxxP macros access the stack space without moving the stack pointer
// ( L5thP, L4thP, L3rdP, L2ndP, TopP, H2ndP, H3rdP, H4thP, H5thP )
//
#define DecP M.pidx--;
//
#define PushR Push(M.rstack,M.ridx)
#define PopR  Pop (M.rstack,M.ridx)
#define TopR  Top (M.rstack,M.ridx)
//
#define PushF Push(M.fstack,M.fidx)
#define PopF  Pop (M.fstack,M.fidx)
#define TopF  Nth (M.fstack,M.fidx, 0)
//
void _dup(void);
void _drop(void);
void _swap(void);
void _over(void);
void _prot (void);
void _nrot (void);
void _2drop(void);
void _2dup(void);
void _2swap(void);
void _qdup(void);
void _onep(void);
void _onen(void);
void _fourp(void);
void _fourn(void);
void _add(void);
void _sub(void);
void _mul(void);
void _div(void);
void _smod(void);
void _equal(void);
void _nequal(void);
void _less(void);
void _greatr(void);
void _lesseq(void);
void _greatreq(void);
void _zeroeq(void);
void _zeroneq(void);
void _zerolth(void);
void _zerogth(void);
void _zltheq(void);
void _zgtheq(void);
void _bitsand(void);
void _bitsor(void);
void _bitsxor(void);
void _bitsnot(void);
//
void _docodetest(void);
//
void _tor(void);
void _fromr(void);
void _rspfetch(void);
void _rspstore(void);
void _rdrop(void);
void _dspfetch(void);
void _dspstore(void);
