#include "Common.h"
#include "StackPrimitives.h"
#include "SystemPrimitives.h"
#include "VFM_Build_Primitives.h"
#ifndef LINUX
#include "picoprimitivesGPIO.h"
#include "picoPrimitivesADC.h"
#include "picoStoreSPI.h"
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
void InsertParameter(int32_t parameter){ // a LOT Like <comma>
   DictEntry *entry = (DictEntry *) &(M.data32[M.LATEST]);
   entry->NumParam++;
   M.data32[M.HERE++] = parameter;
}
//
#ifdef PRINT_DICT_ENTRIES
void Dump(int32_t start){
int32_t count=0;
char buf[16], ac;
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
char buf[40];  
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
  DefHeader( "DUP"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _dup);        InsertParameter((int32_t) _fexit);
  DefHeader( "DROP"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _drop);       InsertParameter((int32_t) _fexit);
  DefHeader( "SWAP"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _swap);       InsertParameter((int32_t) _fexit);
  DefHeader( "OVER"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _over );      InsertParameter((int32_t) _fexit);
  DefHeader( "ROT"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _prot );      InsertParameter((int32_t) _fexit);
  DefHeader( "-ROT"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _nrot);       InsertParameter((int32_t) _fexit);
  DefHeader( "2DROP" , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _2drop);      InsertParameter((int32_t) _fexit);
  DefHeader( "2DUP"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _2dup );      InsertParameter((int32_t) _fexit);
  DefHeader( "2SWAP" , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _2swap);      InsertParameter((int32_t) _fexit);
  DefHeader( "?DUP"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _qdup);       InsertParameter((int32_t) _fexit);
  DefHeader( "1+"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _onep);       InsertParameter((int32_t) _fexit);
  DefHeader( "1-"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _onen);       InsertParameter((int32_t) _fexit);
  DefHeader( "4+"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _fourp);      InsertParameter((int32_t) _fexit);
  DefHeader( "4-"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _fourn);      InsertParameter((int32_t) _fexit);
  DefHeader( "+"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _add);        InsertParameter((int32_t) _fexit);
  DefHeader( "-"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _sub);        InsertParameter((int32_t) _fexit);
  DefHeader( "*"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _mul);        InsertParameter((int32_t) _fexit);
  DefHeader( "/MOD"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _smod);       InsertParameter((int32_t) _fexit);
  DefHeader( "="     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _equal);      InsertParameter((int32_t) _fexit);
  DefHeader( "<>"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _nequal);     InsertParameter((int32_t) _fexit);
  DefHeader( "<"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _less);       InsertParameter((int32_t) _fexit);
  DefHeader( ">"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _greatr);     InsertParameter((int32_t) _fexit);
  DefHeader( "<="    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _lesseq);     InsertParameter((int32_t) _fexit);
  DefHeader( ">="    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _greatreq);   InsertParameter((int32_t) _fexit);
  DefHeader( "0="    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zeroeq);     InsertParameter((int32_t) _fexit);
  DefHeader( "0<>"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zeroneq);    InsertParameter((int32_t) _fexit);
  DefHeader( "0<"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zerolth);    InsertParameter((int32_t) _fexit);
  DefHeader( "0>"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zerogth);    InsertParameter((int32_t) _fexit);
  DefHeader( "0<="   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zltheq);     InsertParameter((int32_t) _fexit);
  DefHeader( "0>="   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zgtheq);     InsertParameter((int32_t) _fexit);
  DefHeader( "AND"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _bitsand);    InsertParameter((int32_t) _fexit);
  DefHeader( "OR"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _bitsor);     InsertParameter((int32_t) _fexit);
  DefHeader( "XOR"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _bitsxor);    InsertParameter((int32_t) _fexit);
  DefHeader( "INVERT", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _bitsnot);    InsertParameter((int32_t) _fexit);
  //
  DefHeader( "!",      NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _store);      InsertParameter((int32_t) _fexit);
  DefHeader( "@",      NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _fetch);      InsertParameter((int32_t) _fexit);
  DefHeader( "+!",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _addstore);   InsertParameter((int32_t) _fexit);
  DefHeader( "-!",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _substore);   InsertParameter((int32_t) _fexit);
  DefHeader( "C!",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _storebyte);  InsertParameter((int32_t) _fexit);
  DefHeader( "C@",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _fetchbyte);  InsertParameter((int32_t) _fexit);
  DefHeader( "C@C!",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _ccopy);      InsertParameter((int32_t) _fexit);
  DefHeader( "CMOVE",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _cmove);      InsertParameter((int32_t) _fexit);
  DefHeader( "KEY",    NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _key);        InsertParameter((int32_t) _fexit);
  DefHeader( "EMIT",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _emit);       InsertParameter((int32_t) _fexit);
  DefHeader( "NUMBER", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _number);     InsertParameter((int32_t) _fexit);
  DefHeader( "FIND",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _find);       InsertParameter((int32_t) _fexit);
//  
#ifndef LINUX
  DefHeader( "TEMPON",    NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_temp_on);           InsertParameter((int32_t) _fexit);
  DefHeader( "TEMPOFF",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_temp_off);          InsertParameter((int32_t) _fexit);
  DefHeader( "ADC_INIT",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_adc_init);          InsertParameter((int32_t) _fexit);
  DefHeader( "ADC_GPIO",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_adc_gpio_init);     InsertParameter((int32_t) _fexit);
  DefHeader( "ADC_INPUT", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_adc_select_input);  InsertParameter((int32_t) _fexit);
  DefHeader( "ADC_READ",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_adc_read);          InsertParameter((int32_t) _fexit);
  //
  DefHeader( "IO_GET_DIR",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_get_dir);                      InsertParameter((int32_t) _fexit);
  DefHeader( "IO_GET_MA",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_get_drive_strength);           InsertParameter((int32_t) _fexit);
  DefHeader( "IO_GET_DT",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_get_slew_rate);                InsertParameter((int32_t) _fexit);
  DefHeader( "IO_NO_PULL",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_disable_pulls);                InsertParameter((int32_t) _fexit);
  DefHeader( "IO_PULLUP",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_pull_up);                      InsertParameter((int32_t) _fexit);
  DefHeader( "IO_PULLDN",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_pull_down);                    InsertParameter((int32_t) _fexit);
  DefHeader( "IO_PULLUP?",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_is_pulled_up);                 InsertParameter((int32_t) _fexit);
  DefHeader( "IO_PULLDN?",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_is_pulled_down);               InsertParameter((int32_t) _fexit);
  DefHeader( "IO_HYSTO?",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_is_input_hysteresis_enabled);  InsertParameter((int32_t) _fexit);
  DefHeader( "IO_GET",      NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_get);                          InsertParameter((int32_t) _fexit);
  DefHeader( "IO_INIT",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_init);                         InsertParameter((int32_t) _fexit);
  DefHeader( "IO_SET_DIR",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_set_dir);                      InsertParameter((int32_t) _fexit);
  DefHeader( "IO_PUT",      NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_put);                          InsertParameter((int32_t) _fexit);
  DefHeader( "IO_SET_MA",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_set_drive_strength);           InsertParameter((int32_t) _fexit);
  DefHeader( "IO_SET_DT",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_set_slew_rate);                InsertParameter((int32_t) _fexit);
  DefHeader( "IO_HYSTO_ON", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_set_input_hysteresis_enabled); InsertParameter((int32_t) _fexit);
  //
  DefHeader( "FLASH_ERASE",    NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_flash_sector_erase);   InsertParameter((int32_t) _fexit);
  DefHeader( "FLASH!",         NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_flash_store);          InsertParameter((int32_t) _fexit);
  DefHeader( "FLASH_LIST",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_flash_page_list);      InsertParameter((int32_t) _fexit);
  DefHeader( "FLASH_ID",       NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_flash_get_unique_id);  InsertParameter((int32_t) _fexit);
  DefHeader( "FLASH_PATRN",    NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_page_pattern);         InsertParameter((int32_t) _fexit);
  DefHeader( "FLASH_WIPE",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_WipeAllSectors);       InsertParameter((int32_t) _fexit);
  DefHeader( "FLASH_GET_LINE", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_Get_Page_Line);        InsertParameter((int32_t) _fexit);
  //
#endif  
}
