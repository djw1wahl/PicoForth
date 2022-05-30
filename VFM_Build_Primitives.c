#include "Common.h"
#include "StackPrimitives.h"
#include "SystemPrimitives.h"
#include "VFM_Build_Primitives.h"
#include "PICOprimitivesGPIO.h"
#include "PicoPrimitivesADC.h"
#include "PicoStoreSPI.h"
//
enum RunTypes { _TYPE0, _TYPE1 }; // these names will change
//
void DefCodeEntry(char* _name, bool _immediate, bool _hidden, int16_t _run, void (*_code)(void)){
int len, padcount=1;
  DictCodeEntry *entry = (DictCodeEntry *) &(M.data32[M.HERE]);
  strcpy(entry->Name, _name);            // null terminated
  len = strlen(_name);
  do{ 
    if((len + padcount) %4 == 0)break;
    entry->Name[len + (padcount++)] =0xFF;
  }while(true);
  entry->Immediate = _immediate;
  entry->Hidden    = _hidden;
  entry->RunType   = _run;
  entry->code      = _code;
  entry->done      = _fexit;
  entry->Link      = M.LATEST;
  M.LATEST = M.HERE;
  M.HERE += (16 + strlen(_name) + padcount) /4;  
}
//
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
void PrintDictEntries(int32_t count){
char buf[40];  
int32_t len, i;
DictCodeEntry *thecode;
 do{
  thecode = (DictCodeEntry *) &(M.data32[M.LATEST]);
  len = strlen(thecode->Name);
               for(i=0;i<len;i++){    sprintf(buf,"%c",thecode->Name[i]);                   PrintBuf(buf); }
               for(   ;i<32 ;i++){    sprintf(buf," ");                                     PrintBuf(buf); }
                                      sprintf(buf," Link = %4X", thecode->Link);            PrintBuf(buf);
  if(thecode->Immediate == false){    sprintf(buf," ~I");                                   PrintBuf(buf); }
  if(thecode->Immediate == true) {    sprintf(buf,"  I");                                   PrintBuf(buf); }
  if(thecode->Hidden    == false){    sprintf(buf," ~H");                                   PrintBuf(buf); }
  if(thecode->Hidden    == true) {    sprintf(buf,"  H");                                   PrintBuf(buf); }  
                                      sprintf(buf,"  Run = %d", thecode->RunType);          PrintBuf(buf);                
                                      sprintf(buf," Code = %8X",thecode->code);             PrintBuf(buf);                                       
                                      sprintf(buf," Done = %8X",thecode->done);             PrintBuf(buf);                                          
                                                                                            PrintBuf("\n");
  if((thecode->Link == 0) && (M.LATEST == 0))break;
  M.LATEST = thecode->Link;
 }while(--count >0);
}
//
void BuildCodeEntries(int32_t where){
  M.HERE = where;
  //            Name      Immed  Hidden  type    code
  DefCodeEntry( "DUP"   , false, false, _TYPE1, _dup); 
  DefCodeEntry( "DROP"  , false, false, _TYPE1, _drop); 
  DefCodeEntry( "SWAP"  , false, false, _TYPE1, _swap); 
  DefCodeEntry( "OVER"  , false, false, _TYPE1, _over );
  DefCodeEntry( "ROT"   , false, false, _TYPE1, _prot );
  DefCodeEntry( "-ROT"  , false, false, _TYPE1, _nrot); 
  DefCodeEntry( "2DROP" , false, false, _TYPE1, _2drop);   
  DefCodeEntry( "2DUP"  , false, false, _TYPE1, _2dup );
  DefCodeEntry( "2SWAP" , false, false, _TYPE1, _2swap);
  DefCodeEntry( "?DUP"  , false, false, _TYPE1, _qdup);
  DefCodeEntry( "1+"    , false, false, _TYPE1, _onep);
  DefCodeEntry( "1-"    , false, false, _TYPE1, _onen);
  DefCodeEntry( "4+"    , false, false, _TYPE1, _fourp);
  DefCodeEntry( "4-"    , false, false, _TYPE1, _fourn);
  DefCodeEntry( "+"     , false, false, _TYPE1, _add);
  DefCodeEntry( "-"     , false, false, _TYPE1, _sub);
  DefCodeEntry( "*"     , false, false, _TYPE1, _mul);
  DefCodeEntry( "/MOD"  , false, false, _TYPE1, _smod);
  DefCodeEntry( "="     , false, false, _TYPE1, _equal);
  DefCodeEntry( "<>"    , false, false, _TYPE1, _nequal);
  DefCodeEntry( "<"     , false, false, _TYPE1, _less);
  DefCodeEntry( ">"     , false, false, _TYPE1, _greatr);
  DefCodeEntry( "<="    , false, false, _TYPE1, _lesseq);
  DefCodeEntry( ">="    , false, false, _TYPE1, _greatreq);
  DefCodeEntry( "0="    , false, false, _TYPE1, _zeroeq);
  DefCodeEntry( "0<>"   , false, false, _TYPE1, _zeroneq);
  DefCodeEntry( "0<"    , false, false, _TYPE1, _zerolth);
  DefCodeEntry( "0>"    , false, false, _TYPE1, _zerogth);
  DefCodeEntry( "0<="   , false, false, _TYPE1, _zltheq);
  DefCodeEntry( "0>="   , false, false, _TYPE1, _zgtheq);
  DefCodeEntry( "AND"   , false, false, _TYPE1, _bitsand);
  DefCodeEntry( "OR"    , false, false, _TYPE1, _bitsor);
  DefCodeEntry( "XOR"   , false, false, _TYPE1, _bitsxor);
  DefCodeEntry( "INVERT", false, false, _TYPE1, _bitsnot);
  //
  DefCodeEntry( "TEMPON",    false, false, _TYPE1, pico_temp_on);
  DefCodeEntry( "TEMPOFF",   false, false, _TYPE1, pico_temp_off);
  DefCodeEntry( "ADC_INIT",  false, false, _TYPE1, pico_adc_init);
  DefCodeEntry( "ADC_GPIO",  false, false, _TYPE1, pico_adc_gpio_init);
  DefCodeEntry( "ADC_INPUT", false, false, _TYPE1, pico_adc_select_input);
  DefCodeEntry( "ADC_READ",  false, false, _TYPE1, pico_adc_read);  
  //
  DefCodeEntry( "IO_GET_DIR",  false, false, _TYPE1, pico_gpio_get_dir);
  DefCodeEntry( "IO_GET_MA",   false, false, _TYPE1, pico_gpio_get_drive_strength);
  DefCodeEntry( "IO_GET_DT",   false, false, _TYPE1, pico_gpio_get_slew_rate);
  DefCodeEntry( "IO_NO_PULL",  false, false, _TYPE1, pico_gpio_disable_pulls);  
  DefCodeEntry( "IO_PULLUP",   false, false, _TYPE1, pico_gpio_pull_up);
  DefCodeEntry( "IO_PULLDN",   false, false, _TYPE1, pico_gpio_pull_down);
  DefCodeEntry( "IO_PULLUP?",  false, false, _TYPE1, pico_gpio_is_pulled_up);
  DefCodeEntry( "IO_PULLDN?",  false, false, _TYPE1, pico_gpio_is_pulled_down);  
  DefCodeEntry( "IO_HYSTO?",   false, false, _TYPE1, pico_gpio_is_input_hysteresis_enabled);
  DefCodeEntry( "IO_GET",      false, false, _TYPE1, pico_gpio_get);
  DefCodeEntry( "IO_INIT",     false, false, _TYPE1, pico_gpio_init);
  DefCodeEntry( "IO_SET_DIR",  false, false, _TYPE1, pico_gpio_set_dir);  
  DefCodeEntry( "IO_PUT",      false, false, _TYPE1, pico_gpio_put);
  DefCodeEntry( "IO_SET_MA",   false, false, _TYPE1, pico_gpio_set_drive_strength);
  DefCodeEntry( "IO_SET_DT",   false, false, _TYPE1, pico_gpio_set_slew_rate);
  DefCodeEntry( "IO_HYSTO_ON", false, false, _TYPE1, pico_gpio_set_input_hysteresis_enabled);  
  //
  DefCodeEntry( "FLASH_ERASE",    false, false, _TYPE1, pico_flash_sector_erase);  
  DefCodeEntry( "FLASH!",         false, false, _TYPE1, pico_flash_store);  
  DefCodeEntry( "FLASH_LIST",     false, false, _TYPE1, pico_flash_page_list);  
  DefCodeEntry( "FLASH_ID",       false, false, _TYPE1, pico_flash_get_unique_id);  
  DefCodeEntry( "FLASH_PATRN",    false, false, _TYPE1, pico_page_pattern);  
  DefCodeEntry( "FLASH_WIPE",     false, false, _TYPE1, pico_WipeAllSectors);  
  DefCodeEntry( "FLASH_GET_LINE", false, false, _TYPE1, pico_Get_Page_Line);  
}
