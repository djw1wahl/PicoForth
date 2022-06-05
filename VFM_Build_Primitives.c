#include "Common.h"
#include "StackPrimitives.h"
#include "SystemPrimitives.h"
#include "VFM_Build_Primitives.h"
#ifdef PICO
#include "PicoPrimitivesGPIO.h"
#include "PicoPrimitivesADC.h"
#include "PicoStoreSPI.h"
#endif
//
void DefHeader(char* _name, int32_t _immediate, int32_t _hidden){
int32_t len, padcount=1;
  DictEntry *entry = (DictEntry *) &(M.data32[M.HERE]);
  strcpy(entry->Name, _name);            // null terminated
  len = strlen(_name);
  entry->Len = len;
  do{ 
    if((len + padcount) %4 == 0)break;
    entry->Name[len + (padcount++)] = 0xFF;
  }while(true);
  entry->Immediate |= _immediate;
  entry->Hidden    |= _hidden;
  entry->Link       = M.LATEST;
  M.LATEST          = M.HERE;
  M.HERE           += ( sizeof(entry->Link) + \
                        sizeof(entry->Len) + \
                        sizeof(entry->Immediate) + \
                        sizeof(entry->Hidden) + \
                        sizeof(entry->NumParam) + \
                        sizeof(entry->pfa) + \
                        strlen(_name) + 
                        padcount ) /4;  
  entry->pfa        = M.HERE;
}
//
void InsertCode(int32_t parameter){ // a LOT Like <comma>
  DictEntry *entry = (DictEntry *) &(M.data32[M.LATEST]);
  entry->NumParam++;
  M.data32[M.HERE++] = parameter;
}
//
void InsertWord(char* TheName){ 
  PushP = (int32_t)TheName; // put a word name on the stack
  _find();                                
  if(TopP == 0){ sprintf(buf,"InsertWord can't find that name: %s\n", TheName); PrintBuf(buf); return; } 
  DictEntry *entry = (DictEntry *) &(M.data32[M.LATEST]);
  entry->NumParam++;                              
  M.data32[M.HERE++] = PopP;
}
//
#ifdef PRINT_DICT_ENTRIES
void Dump(int32_t start){
int32_t count=0;
char ac;
union{
  int32_t val;
  char cval[4];
}vals;
  do{
    sprintf(buf," %4.4X %8.8X ",start, M.data32[start]); PrintBuf(buf);
    vals.val = M.data32[start];
    if(M.data32[start++] == 0){ count++; }else { count = 0; }
    for(int k=0;k<4;k++){
      ac = vals.cval[k];
      if((ac>32) && (ac<126)){ sprintf(buf,"%1c",ac); PrintBuf(buf); }
      else                   { sprintf(buf," "); PrintBuf(buf); }      
    }
    PrintBuf("\n");
    if(start>=MEMSIZE)count=8;
  }while(count<8); // stops after 8 consecutive empty locations or end of memory
}
//
void PrintDictEntries(void){
int32_t len, i, j;
bool alldone = false;
DictEntry *thecode;
 do{
  thecode = (DictEntry *) &(M.data32[M.LATEST]);
  if(thecode->Link == 0)alldone = true;
  len = strlen(thecode->Name);
               for(i=0;i<len;i++){    sprintf(buf,"%c",thecode->Name[i]);                   PrintBuf(buf); }
               for(   ;i<32 ;i++){    sprintf(buf," ");                                     PrintBuf(buf); }
                                      sprintf(buf," Link = %4X", thecode->Link);            PrintBuf(buf);
                                      sprintf(buf," Len  = %3d", thecode->Len & LENMASK);   PrintBuf(buf);                                      
  if((thecode->Immediate & IMMED) == 0){  sprintf(buf," ~I");                               PrintBuf(buf); }
  if((thecode->Immediate & IMMED) != 0){  sprintf(buf,"  I");                               PrintBuf(buf); }
  if((thecode->Hidden    & HIDEN) == 0){  sprintf(buf," ~H");                               PrintBuf(buf); }
  if((thecode->Hidden    & HIDEN) != 0){  sprintf(buf,"  H");                               PrintBuf(buf); }  
                            sprintf(buf," (PFA) = %8X", (uint32_t)thecode->pfa);            PrintBuf(buf); 
  j = thecode->pfa;                          
  for(i=0;i<thecode->NumParam;i++){
    sprintf(buf," P[%2d] = %8X", i, M.data32[j++]); PrintBuf(buf);
  }
  PrintBuf("\n");
  M.LATEST = thecode->Link;
 }while(!alldone);
}
#endif
//
void BuildCodeEntries(int32_t where){
  M.HERE = where;
  //            Name      Immed  Hidden  type    code
  DefHeader( "DUP"   , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _dup);        InsertCode((int32_t) _fexit);
  DefHeader( "DROP"  , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _drop);       InsertCode((int32_t) _fexit);
  DefHeader( "SWAP"  , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _swap);       InsertCode((int32_t) _fexit);
  DefHeader( "OVER"  , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _over );      InsertCode((int32_t) _fexit);
  DefHeader( "ROT"   , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _prot );      InsertCode((int32_t) _fexit);
  DefHeader( "-ROT"  , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _nrot);       InsertCode((int32_t) _fexit);
  DefHeader( "2DROP" , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _2drop);      InsertCode((int32_t) _fexit);
  DefHeader( "2DUP"  , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _2dup );      InsertCode((int32_t) _fexit);
  DefHeader( "2SWAP" , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _2swap);      InsertCode((int32_t) _fexit);
  DefHeader( "?DUP"  , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _qdup);       InsertCode((int32_t) _fexit);
  DefHeader( "1+"    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _onep);       InsertCode((int32_t) _fexit);
  DefHeader( "1-"    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _onen);       InsertCode((int32_t) _fexit);
  DefHeader( "4+"    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _fourp);      InsertCode((int32_t) _fexit);
  DefHeader( "4-"    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _fourn);      InsertCode((int32_t) _fexit);
  DefHeader( "+"     , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _add);        InsertCode((int32_t) _fexit);
  DefHeader( "-"     , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _sub);        InsertCode((int32_t) _fexit);
  DefHeader( "*"     , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _mul);        InsertCode((int32_t) _fexit);
  DefHeader( "/MOD"  , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _smod);       InsertCode((int32_t) _fexit);
  DefHeader( "="     , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _equal);      InsertCode((int32_t) _fexit);
  DefHeader( "<>"    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _nequal);     InsertCode((int32_t) _fexit);
  DefHeader( "<"     , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _less);       InsertCode((int32_t) _fexit);
  DefHeader( ">"     , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _greatr);     InsertCode((int32_t) _fexit);
  DefHeader( "<="    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _lesseq);     InsertCode((int32_t) _fexit);
  DefHeader( ">="    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _greatreq);   InsertCode((int32_t) _fexit);
  DefHeader( "0="    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _zeroeq);     InsertCode((int32_t) _fexit);
  DefHeader( "0<>"   , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _zeroneq);    InsertCode((int32_t) _fexit);
  DefHeader( "0<"    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _zerolth);    InsertCode((int32_t) _fexit);
  DefHeader( "0>"    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _zerogth);    InsertCode((int32_t) _fexit);
  DefHeader( "0<="   , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _zltheq);     InsertCode((int32_t) _fexit);
  DefHeader( "0>="   , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _zgtheq);     InsertCode((int32_t) _fexit);
  DefHeader( "AND"   , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _bitsand);    InsertCode((int32_t) _fexit);
  DefHeader( "OR"    , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _bitsor);     InsertCode((int32_t) _fexit);
  DefHeader( "XOR"   , NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _bitsxor);    InsertCode((int32_t) _fexit);
  DefHeader( "INVERT", NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _bitsnot);    InsertCode((int32_t) _fexit);
  //
  DefHeader( "!",      NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _store);      InsertCode((int32_t) _fexit);
  DefHeader( "@",      NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _fetch);      InsertCode((int32_t) _fexit);
  DefHeader( "+!",     NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _addstore);   InsertCode((int32_t) _fexit);
  DefHeader( "-!",     NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _substore);   InsertCode((int32_t) _fexit);
  DefHeader( "C!",     NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _storebyte);  InsertCode((int32_t) _fexit);
  DefHeader( "C@",     NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _fetchbyte);  InsertCode((int32_t) _fexit);
  DefHeader( "C@C!",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _ccopy);      InsertCode((int32_t) _fexit);
  DefHeader( "CMOVE",  NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _cmove);      InsertCode((int32_t) _fexit);
  DefHeader( "KEY",    NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _key);        InsertCode((int32_t) _fexit);
  DefHeader( "EMIT",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _emit);       InsertCode((int32_t) _fexit);
  DefHeader( "NUMBER", NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _number);     InsertCode((int32_t) _fexit);
  DefHeader( "FIND",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) _find);       InsertCode((int32_t) _fexit);
//  
#ifdef PICO
  DefHeader( "TEMPON",    NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_temp_on);           InsertCode((int32_t) _fexit);
  DefHeader( "TEMPOFF",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_temp_off);          InsertCode((int32_t) _fexit);
  DefHeader( "ADC_INIT",  NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_adc_init);          InsertCode((int32_t) _fexit);
  DefHeader( "ADC_GPIO",  NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_adc_gpio_init);     InsertCode((int32_t) _fexit);
  DefHeader( "ADC_INPUT", NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_adc_select_input);  InsertCode((int32_t) _fexit);
  DefHeader( "ADC_READ",  NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_adc_read);          InsertCode((int32_t) _fexit);
  //
  DefHeader( "IO_GET_DIR",  NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_get_dir);                      InsertCode((int32_t) _fexit);
  DefHeader( "IO_GET_MA",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_get_drive_strength);           InsertCode((int32_t) _fexit);
  DefHeader( "IO_GET_DT",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_get_slew_rate);                InsertCode((int32_t) _fexit);
  DefHeader( "IO_NO_PULL",  NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_disable_pulls);                InsertCode((int32_t) _fexit);
  DefHeader( "IO_PULLUP",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_pull_up);                      InsertCode((int32_t) _fexit);
  DefHeader( "IO_PULLDN",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_pull_down);                    InsertCode((int32_t) _fexit);
  DefHeader( "IO_PULLUP?",  NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_is_pulled_up);                 InsertCode((int32_t) _fexit);
  DefHeader( "IO_PULLDN?",  NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_is_pulled_down);               InsertCode((int32_t) _fexit);
  DefHeader( "IO_HYSTO?",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_is_input_hysteresis_enabled);  InsertCode((int32_t) _fexit);
  DefHeader( "IO_GET",      NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_get);                          InsertCode((int32_t) _fexit);
  DefHeader( "IO_INIT",     NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_init);                         InsertCode((int32_t) _fexit);
  DefHeader( "IO_SET_DIR",  NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_set_dir);                      InsertCode((int32_t) _fexit);
  DefHeader( "IO_PUT",      NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_put);                          InsertCode((int32_t) _fexit);
  DefHeader( "IO_SET_MA",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_set_drive_strength);           InsertCode((int32_t) _fexit);
  DefHeader( "IO_SET_DT",   NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_set_slew_rate);                InsertCode((int32_t) _fexit);
  DefHeader( "IO_HYSTO_ON", NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_gpio_set_input_hysteresis_enabled); InsertCode((int32_t) _fexit);
  //
  DefHeader( "FLASH_ERASE",    NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_flash_sector_erase);   InsertCode((int32_t) _fexit);
  DefHeader( "FLASH!",         NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_flash_store);          InsertCode((int32_t) _fexit);
  DefHeader( "FLASH_LIST",     NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_flash_page_list);      InsertCode((int32_t) _fexit);
  DefHeader( "FLASH_ID",       NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_flash_get_unique_id);  InsertCode((int32_t) _fexit);
  DefHeader( "FLASH_PATRN",    NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_page_pattern);         InsertCode((int32_t) _fexit);
  DefHeader( "FLASH_WIPE",     NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_WipeAllSectors);       InsertCode((int32_t) _fexit);
  DefHeader( "FLASH_GET_LINE", NADAZ, NADAZ); InsertCode((int32_t) _docode); InsertCode((int32_t) pico_Get_Page_Line);        InsertCode((int32_t) _fexit);
  //
#endif  
}
//
void BuildWordEntries(int32_t where){
  DefHeader( "NEXT", NADAZ, NADAZ); InsertCode((int32_t) _docolon); InsertWord("SWAP"); 
  DefHeader( "DOG",  NADAZ, NADAZ); InsertCode((int32_t) _docolon); InsertWord("SWAP"); InsertWord("NEXT");
}