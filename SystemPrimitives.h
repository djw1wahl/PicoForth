void _fexit(void);
void _fnext(void);
void _dothis(void);
//
void _store(void);
void _fetch(void);
void _addstore(void);
void _substore(void);
void _storebyte(void);
void _fetchbyte(void);
void _ccopy(void);
void _cmove(void);
void _key(void);
void _emit(void);
void _word(void);
//
void _number(void);
#ifdef TEST_NUMBER
  void testnumber(void);
#endif
void _find(void);
#ifdef TEST_FIND
  void testfind(char*);
#endif
//
void _tcfa(void);
void _tdfa(void);
void _interpret(void);
void _create(void);
void _comma(void);
void _lbrac(void);
void _rbrac(void);
void _colon(void);
void _semicolon(void);
void _immediate(void);
void _hidden(void);
void _hide(void);
void _tick(void);
void _branch(void);
void _zbranch(void);
void _litstring(void);
void _tell(void);
void _quit(void);
void _char(void);
void _execute(void);
