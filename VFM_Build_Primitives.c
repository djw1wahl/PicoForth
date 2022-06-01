#include "Common.h"
#include "StackPrimitives.h"
#include "SystemPrimitives.h"
#include "VFM_Build_Primitives.h"
#include "picoprimitivesGPIO.h"
#include "picoPrimitivesADC.h"
#include "picoStoreSPI.h"
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
  DefHeader( "DUP"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _dup); 
  DefHeader( "DROP"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _drop); 
  DefHeader( "SWAP"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _swap); 
  DefHeader( "OVER"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _over );
  DefHeader( "ROT"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _prot );
  DefHeader( "-ROT"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _nrot); 
  DefHeader( "2DROP" , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _2drop);   
  DefHeader( "2DUP"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _2dup );
  DefHeader( "2SWAP" , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _2swap);
  DefHeader( "?DUP"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _qdup);
  DefHeader( "1+"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _onep);
  DefHeader( "1-"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _onen);
  DefHeader( "4+"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _fourp);
  DefHeader( "4-"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _fourn);
  DefHeader( "+"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _add);
  DefHeader( "-"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _sub);
  DefHeader( "*"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _mul);
  DefHeader( "/MOD"  , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _smod);
  DefHeader( "="     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _equal);
  DefHeader( "<>"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _nequal);
  DefHeader( "<"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _less);
  DefHeader( ">"     , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _greatr);
  DefHeader( "<="    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _lesseq);
  DefHeader( ">="    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _greatreq);
  DefHeader( "0="    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zeroeq);
  DefHeader( "0<>"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zeroneq);
  DefHeader( "0<"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zerolth);
  DefHeader( "0>"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zerogth);
  DefHeader( "0<="   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zltheq);
  DefHeader( "0>="   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _zgtheq);
  DefHeader( "AND"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _bitsand);
  DefHeader( "OR"    , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _bitsor);
  DefHeader( "XOR"   , NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _bitsxor);
  DefHeader( "INVERT", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _bitsnot);
  //
  DefHeader( "TEMPON",    NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_temp_on);
  DefHeader( "TEMPOFF",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_temp_off);
  DefHeader( "ADC_INIT",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_adc_init);
  DefHeader( "ADC_GPIO",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_adc_gpio_init);
  DefHeader( "ADC_INPUT", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_adc_select_input);
  DefHeader( "ADC_READ",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_adc_read);  
  //
  DefHeader( "IO_GET_DIR",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_get_dir);
  DefHeader( "IO_GET_MA",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_get_drive_strength);
  DefHeader( "IO_GET_DT",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_get_slew_rate);
  DefHeader( "IO_NO_PULL",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_disable_pulls);  
  DefHeader( "IO_PULLUP",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_pull_up);
  DefHeader( "IO_PULLDN",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_pull_down);
  DefHeader( "IO_PULLUP?",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_is_pulled_up);
  DefHeader( "IO_PULLDN?",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_is_pulled_down);  
  DefHeader( "IO_HYSTO?",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_is_input_hysteresis_enabled);
  DefHeader( "IO_GET",      NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_get);
  DefHeader( "IO_INIT",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_init);
  DefHeader( "IO_SET_DIR",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_set_dir);  
  DefHeader( "IO_PUT",      NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_put);
  DefHeader( "IO_SET_MA",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_set_drive_strength);
  DefHeader( "IO_SET_DT",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_set_slew_rate);
  DefHeader( "IO_HYSTO_ON", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_gpio_set_input_hysteresis_enabled);  
  //
  DefHeader( "FLASH_ERASE",    NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_flash_sector_erase);  
  DefHeader( "FLASH!",         NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_flash_store);  
  DefHeader( "FLASH_LIST",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_flash_page_list);  
  DefHeader( "FLASH_ID",       NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_flash_get_unique_id);  
  DefHeader( "FLASH_PATRN",    NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_page_pattern);  
  DefHeader( "FLASH_WIPE",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_WipeAllSectors);  
  DefHeader( "FLASH_GET_LINE", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) pico_Get_Page_Line);  
  //
  DefHeader( "!",      NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _store);   
  DefHeader( "@",      NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _fetch);  
  DefHeader( "+!",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _addstore);  
  DefHeader( "-!",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _substore);  
  DefHeader( "C!",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _storebyte);   
  DefHeader( "C@",     NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _fetchbyte);  
  DefHeader( "C@C!",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _ccopy);  
  DefHeader( "CMOVE",  NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _cmove);  
  DefHeader( "KEY",    NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _key);   
  DefHeader( "EMIT",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _emit);  
  DefHeader( "NUMBER", NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _number);  
  DefHeader( "FIND",   NADAZ, NADAZ); InsertParameter((int32_t) _dothis); InsertParameter((int32_t) _find);    
  //    
}
