#include "Common.h"
#include "StackPrimitives.h"
#include "SystemPrimitives.h"
#include "VFM_Build_Primitives.h"
#include "PICOprimitivesGPIO.h"
#include "PicoPrimitivesADC.h"
#include "PicoStoreSPI.h"
//
void DefCodeEntry(char* _name, int32_t _immediate, int32_t _hidden, void (*_run)(void), void (*_code)(void)){
int32_t len, padcount=1, tmp;
  DictCodeEntry *entry = (DictCodeEntry *) &(M.data32[M.HERE]);
  strcpy(entry->Name, _name);            // null terminated
  len = strlen(_name);
  entry->Len = len;
  do{ 
    if((len + padcount) %4 == 0)break;
    entry->Name[len + (padcount++)] = 0xFF;
  }while(true);
  entry->Immediate |= _immediate;
  entry->Hidden    |= _hidden;
  entry->cfa       = _run;
  entry->code      = _code;
  entry->done      = _fexit;
  entry->Link      = M.LATEST;
  M.LATEST         = M.HERE;
  tmp              = sizeof(entry->Link) + sizeof(entry->Immediate);
  tmp             += sizeof(entry->cfa)  + sizeof(entry->code) + sizeof(entry->done);
  M.HERE          += ( tmp + strlen(_name) + padcount ) /4;  
}
//
#ifdef PRINT_DICT_ENTRIES
void Dump(int32_t start, int32_t count){
char buf [16], ac;
union{
  int32_t val;
  char cval[4];
}vals;
  for(int i=start;i<count;i++){
    sprintf(buf," %4.4X %8.8X ",i, M.data32[i]); PrintBuf(buf);
    vals.val = M.data32[i];
    for(int k=0;k<4;k++){
      ac = vals.cval[k];
      if((ac>32) && (ac<126)){ sprintf(buf,"%1c",ac); PrintBuf(buf); }
      else                   { sprintf(buf," "); PrintBuf(buf); }      
    }
    PrintBuf("\n");
  }
}
//
void PrintDictEntries(void){
char buf[40];  
int32_t len, i;
bool alldone = false;
DictCodeEntry *thecode;
 do{
  thecode = (DictCodeEntry *) &(M.data32[M.LATEST]);
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
                                      sprintf(buf,"   Do = %8X", thecode->cfa);             PrintBuf(buf);                
                                      sprintf(buf," Code = %8X",thecode->code);             PrintBuf(buf);                                       
                                      sprintf(buf," Done = %8X",thecode->done);             PrintBuf(buf);                                          
                                                                                            PrintBuf("\n");
  M.LATEST = thecode->Link;
 }while(!alldone);
}
#endif
//
void BuildCodeEntries(int32_t where){
  M.HERE = where;
  //            Name      Immed  Hidden  type    code
  DefCodeEntry( "DUP"   , NADAZ, NADAZ, _dothis, _dup); 
  DefCodeEntry( "DROP"  , NADAZ, NADAZ, _dothis, _drop); 
  DefCodeEntry( "SWAP"  , NADAZ, NADAZ, _dothis, _swap); 
  DefCodeEntry( "OVER"  , NADAZ, NADAZ, _dothis, _over );
  DefCodeEntry( "ROT"   , NADAZ, NADAZ, _dothis, _prot );
  DefCodeEntry( "-ROT"  , NADAZ, NADAZ, _dothis, _nrot); 
  DefCodeEntry( "2DROP" , NADAZ, NADAZ, _dothis, _2drop);   
  DefCodeEntry( "2DUP"  , NADAZ, NADAZ, _dothis, _2dup );
  DefCodeEntry( "2SWAP" , NADAZ, NADAZ, _dothis, _2swap);
  DefCodeEntry( "?DUP"  , NADAZ, NADAZ, _dothis, _qdup);
  DefCodeEntry( "1+"    , NADAZ, NADAZ, _dothis, _onep);
  DefCodeEntry( "1-"    , NADAZ, NADAZ, _dothis, _onen);
  DefCodeEntry( "4+"    , NADAZ, NADAZ, _dothis, _fourp);
  DefCodeEntry( "4-"    , NADAZ, NADAZ, _dothis, _fourn);
  DefCodeEntry( "+"     , NADAZ, NADAZ, _dothis, _add);
  DefCodeEntry( "-"     , NADAZ, NADAZ, _dothis, _sub);
  DefCodeEntry( "*"     , NADAZ, NADAZ, _dothis, _mul);
  DefCodeEntry( "/MOD"  , NADAZ, NADAZ, _dothis, _smod);
  DefCodeEntry( "="     , NADAZ, NADAZ, _dothis, _equal);
  DefCodeEntry( "<>"    , NADAZ, NADAZ, _dothis, _nequal);
  DefCodeEntry( "<"     , NADAZ, NADAZ, _dothis, _less);
  DefCodeEntry( ">"     , NADAZ, NADAZ, _dothis, _greatr);
  DefCodeEntry( "<="    , NADAZ, NADAZ, _dothis, _lesseq);
  DefCodeEntry( ">="    , NADAZ, NADAZ, _dothis, _greatreq);
  DefCodeEntry( "0="    , NADAZ, NADAZ, _dothis, _zeroeq);
  DefCodeEntry( "0<>"   , NADAZ, NADAZ, _dothis, _zeroneq);
  DefCodeEntry( "0<"    , NADAZ, NADAZ, _dothis, _zerolth);
  DefCodeEntry( "0>"    , NADAZ, NADAZ, _dothis, _zerogth);
  DefCodeEntry( "0<="   , NADAZ, NADAZ, _dothis, _zltheq);
  DefCodeEntry( "0>="   , NADAZ, NADAZ, _dothis, _zgtheq);
  DefCodeEntry( "AND"   , NADAZ, NADAZ, _dothis, _bitsand);
  DefCodeEntry( "OR"    , NADAZ, NADAZ, _dothis, _bitsor);
  DefCodeEntry( "XOR"   , NADAZ, NADAZ, _dothis, _bitsxor);
  DefCodeEntry( "INVERT", NADAZ, NADAZ, _dothis, _bitsnot);
  //
  DefCodeEntry( "TEMPON",    NADAZ, NADAZ, _dothis, pico_temp_on);
  DefCodeEntry( "TEMPOFF",   NADAZ, NADAZ, _dothis, pico_temp_off);
  DefCodeEntry( "ADC_INIT",  NADAZ, NADAZ, _dothis, pico_adc_init);
  DefCodeEntry( "ADC_GPIO",  NADAZ, NADAZ, _dothis, pico_adc_gpio_init);
  DefCodeEntry( "ADC_INPUT", NADAZ, NADAZ, _dothis, pico_adc_select_input);
  DefCodeEntry( "ADC_READ",  NADAZ, NADAZ, _dothis, pico_adc_read);  
  //
  DefCodeEntry( "IO_GET_DIR",  NADAZ, NADAZ, _dothis, pico_gpio_get_dir);
  DefCodeEntry( "IO_GET_MA",   NADAZ, NADAZ, _dothis, pico_gpio_get_drive_strength);
  DefCodeEntry( "IO_GET_DT",   NADAZ, NADAZ, _dothis, pico_gpio_get_slew_rate);
  DefCodeEntry( "IO_NO_PULL",  NADAZ, NADAZ, _dothis, pico_gpio_disable_pulls);  
  DefCodeEntry( "IO_PULLUP",   NADAZ, NADAZ, _dothis, pico_gpio_pull_up);
  DefCodeEntry( "IO_PULLDN",   NADAZ, NADAZ, _dothis, pico_gpio_pull_down);
  DefCodeEntry( "IO_PULLUP?",  NADAZ, NADAZ, _dothis, pico_gpio_is_pulled_up);
  DefCodeEntry( "IO_PULLDN?",  NADAZ, NADAZ, _dothis, pico_gpio_is_pulled_down);  
  DefCodeEntry( "IO_HYSTO?",   NADAZ, NADAZ, _dothis, pico_gpio_is_input_hysteresis_enabled);
  DefCodeEntry( "IO_GET",      NADAZ, NADAZ, _dothis, pico_gpio_get);
  DefCodeEntry( "IO_INIT",     NADAZ, NADAZ, _dothis, pico_gpio_init);
  DefCodeEntry( "IO_SET_DIR",  NADAZ, NADAZ, _dothis, pico_gpio_set_dir);  
  DefCodeEntry( "IO_PUT",      NADAZ, NADAZ, _dothis, pico_gpio_put);
  DefCodeEntry( "IO_SET_MA",   NADAZ, NADAZ, _dothis, pico_gpio_set_drive_strength);
  DefCodeEntry( "IO_SET_DT",   NADAZ, NADAZ, _dothis, pico_gpio_set_slew_rate);
  DefCodeEntry( "IO_HYSTO_ON", NADAZ, NADAZ, _dothis, pico_gpio_set_input_hysteresis_enabled);  
  //
  DefCodeEntry( "FLASH_ERASE",    NADAZ, NADAZ, _dothis, pico_flash_sector_erase);  
  DefCodeEntry( "FLASH!",         NADAZ, NADAZ, _dothis, pico_flash_store);  
  DefCodeEntry( "FLASH_LIST",     NADAZ, NADAZ, _dothis, pico_flash_page_list);  
  DefCodeEntry( "FLASH_ID",       NADAZ, NADAZ, _dothis, pico_flash_get_unique_id);  
  DefCodeEntry( "FLASH_PATRN",    NADAZ, NADAZ, _dothis, pico_page_pattern);  
  DefCodeEntry( "FLASH_WIPE",     NADAZ, NADAZ, _dothis, pico_WipeAllSectors);  
  DefCodeEntry( "FLASH_GET_LINE", NADAZ, NADAZ, _dothis, pico_Get_Page_Line);  
  //
  DefCodeEntry( "!",      NADAZ, NADAZ, _dothis, _store);   
  DefCodeEntry( "@",      NADAZ, NADAZ, _dothis, _fetch);  
  DefCodeEntry( "+!",     NADAZ, NADAZ, _dothis, _addstore);  
  DefCodeEntry( "-!",     NADAZ, NADAZ, _dothis, _substore);  
  DefCodeEntry( "C!",     NADAZ, NADAZ, _dothis, _storebyte);   
  DefCodeEntry( "C@",     NADAZ, NADAZ, _dothis, _fetchbyte);  
  DefCodeEntry( "C@C!",   NADAZ, NADAZ, _dothis, _ccopy);  
  DefCodeEntry( "CMOVE",  NADAZ, NADAZ, _dothis, _cmove);  
  DefCodeEntry( "KEY",    NADAZ, NADAZ, _dothis, _key);   
  DefCodeEntry( "EMIT",   NADAZ, NADAZ, _dothis, _emit);  
  DefCodeEntry( "NUMBER", NADAZ, NADAZ, _dothis, _number);  
  DefCodeEntry( "FIND",   NADAZ, NADAZ, _dothis, _find);    
  //    
}
