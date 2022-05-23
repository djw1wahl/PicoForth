//
// stack index is limited to 0:255 which creates a circular stack
// the present stack index value points to the top of the stack
//
#define Push(x,y) (x[++y])
#define Pop(x,y)  (x[y--])
#define Top(x,y)  (x[y])
//
#define PushP Push(M.pstack,M.pidx)
// usage: PushP = value;
#define PopP  Pop (M.pstack,M.pidx)
// usage: value = PopP
#define TopP  Top (M.pstack,M.pidx)
// usage: value = TopP; 
//        TopP  = value;
//
#define PushR Push(M.rstack,M.ridx)
#define PopR  Pop (M.rstack,M.ridx)
#define TopR  Top (M.rstack,M.ridx)
//
#define PushF Push(M.fstack,M.fidx)
#define PopF  Pop (M.fstack,M.fidx)
#define TopF  Top (M.fstack,M.fidx)
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
void _smod(void);
void _equal(void);
void _nequal(void);
