#include "Common.h"
#include "StackPrimitives.h"
#include "SystemPrimitives.h"
#include "VFM_Build_Primitives.h"
#include "VFM.h"
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
  entry->Flags     |= _immediate;
  entry->Flags     |= _hidden;
  entry->PC         = 0;  
  entry->Link       = M.LATEST;
  M.LATEST          = M.HERE;
  M.HERE           += ( sizeof(entry->Link) + \
                        sizeof(entry->Len) + \
                        sizeof(entry->Flags) + \
                        sizeof(entry->PC) + \
                        sizeof(entry->NumParam) + \
                        sizeof(entry->cfa) + \
                        strlen(_name) + 
                        padcount ) /4;  
  entry->cfa        = M.HERE;
}
//
void Insert(int32_t parameter){ // a LOT Like <comma>
  DictEntry *entry = (DictEntry *) &(M.data32[M.LATEST]);  
  entry->NumParam++;
  M.data32[M.HERE++] = parameter;
}
//
void InsertWord(char* TheName){ 
  PushP = (int32_t)TheName; // put a word name on the stack
  _find();                                
  if(TopP == 0){ sprintf(buf,"InsertWord can't find that name: %s\n", TheName); PrintBuf(buf); return; } 
  int32_t WordHead = PopP;
  DictEntry *_head = (DictEntry *) &(M.data32[WordHead]);   
  Insert((int32_t) _head); // we insert the adr of the target DictEntry                     
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
    sprintf(buf," %4.4X %8.8X %8.8X ",start, (int32_t) &M.data32[start], M.data32[start]); PrintBuf(buf);
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
DictEntry* ViewEntry(char* astring){
  char flags[20], dashs[32];
  DictEntry *entry, *ptr;
  PushP = (int32_t) astring;
  _find();
  int32_t idx = PopP;
  entry = (DictEntry*) &M.data32[idx];
  if(idx == 0){ 
    sprintf(buf,"%s Not Found.\n", astring); PrintBuf(buf);
    return((DictEntry*)0);
  }
  sprintf(buf,"--------------------------%s", entry->Name); PrintBuf(buf); 
  strcpy(dashs,"");
  for(int32_t i=0;i<32-entry->Len;i++){ strcat(dashs,"-") ; }
  PrintBuf(dashs); 
  PrintBuf("\n");
  //
  strcpy(flags, "");
  if(entry->Flags & IMMED){ strcat(flags,"Immediate "); }
  if(entry->Flags & HIDEN){ strcat(flags,"Hidden ");    }  
  //
  sprintf(dashs,"%8.8X",(int32_t) &M.data32[entry->Link]);
  sprintf(buf,      "%s   Link: %8.8X \n", dashs, entry->Link);     PrintBuf(buf);  
  sprintf(buf,"           Name: %s\n"    , entry->Name   );         PrintBuf(buf);
  sprintf(buf,"Memory  Address: %8.8X \n", (int32_t)entry);         PrintBuf(buf);
  sprintf(buf,"Memory    Index: %8.8X \n", idx);                    PrintBuf(buf);
  sprintf(buf,"         Length: %2.2d \n", entry->Len    );         PrintBuf(buf);
  sprintf(buf,"          Flags: %s \n", flags);                     PrintBuf(buf);
  sprintf(buf,"           Type: %s \n", "Unused");                  PrintBuf(buf);
  sprintf(buf,"Parameter Count: %2.2d \n",entry->NumParam );        PrintBuf(buf);
  sprintf(buf,"            CFA: %8.8X \n",entry->cfa );             PrintBuf(buf);
//
  return(entry);
}
//
void PrintDictEntries(char* From, char* Too ){
DictEntry* entry;
char name_buf[32];
  strcpy(name_buf,From);
  do{
    entry = ViewEntry(name_buf);
    entry = (DictEntry*) &M.data32[entry->Link];
    strcpy(name_buf, entry->Name);
  }while(strcmp(name_buf, Too) != 0);
}
#endif
//
void BuildCodeEntries(int32_t where){
  M.HERE = where;
  //            Name      Immed  Hidden  type    code
  DefHeader( "DUP"   , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _dup);        Insert((int32_t) _rtn);
  DefHeader( "DROP"  , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _drop);       Insert((int32_t) _rtn);
  DefHeader( "SWAP"  , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _swap);       Insert((int32_t) _rtn);
  DefHeader( "OVER"  , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _over );      Insert((int32_t) _rtn);
  DefHeader( "ROT"   , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _prot );      Insert((int32_t) _rtn);
  DefHeader( "-ROT"  , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _nrot);       Insert((int32_t) _rtn);
  DefHeader( "2DROP" , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _2drop);      Insert((int32_t) _rtn);
  DefHeader( "2DUP"  , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _2dup );      Insert((int32_t) _rtn);
  DefHeader( "2SWAP" , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _2swap);      Insert((int32_t) _rtn);
  DefHeader( "?DUP"  , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _qdup);       Insert((int32_t) _rtn);
  DefHeader( "1+"    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _onep);       Insert((int32_t) _rtn);
  DefHeader( "1-"    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _onen);       Insert((int32_t) _rtn);
  DefHeader( "4+"    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _fourp);      Insert((int32_t) _rtn);
  DefHeader( "4-"    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _fourn);      Insert((int32_t) _rtn);
  DefHeader( "+"     , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _add);        Insert((int32_t) _rtn);
  DefHeader( "-"     , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _sub);        Insert((int32_t) _rtn);
  DefHeader( "*"     , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _mul);        Insert((int32_t) _rtn);
  DefHeader( "/MOD"  , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _smod);       Insert((int32_t) _rtn);
  DefHeader( "="     , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _equal);      Insert((int32_t) _rtn);
  DefHeader( "<>"    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _nequal);     Insert((int32_t) _rtn);
  DefHeader( "<"     , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _less);       Insert((int32_t) _rtn);
  DefHeader( ">"     , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _greatr);     Insert((int32_t) _rtn);
  DefHeader( "<="    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _lesseq);     Insert((int32_t) _rtn);
  DefHeader( ">="    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _greatreq);   Insert((int32_t) _rtn);
  DefHeader( "0="    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _zeroeq);     Insert((int32_t) _rtn);
  DefHeader( "0<>"   , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _zeroneq);    Insert((int32_t) _rtn);
  DefHeader( "0<"    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _zerolth);    Insert((int32_t) _rtn);
  DefHeader( "0>"    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _zerogth);    Insert((int32_t) _rtn);
  DefHeader( "0<="   , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _zltheq);     Insert((int32_t) _rtn);
  DefHeader( "0>="   , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _zgtheq);     Insert((int32_t) _rtn);
  DefHeader( "AND"   , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _bitsand);    Insert((int32_t) _rtn);
  DefHeader( "OR"    , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _bitsor);     Insert((int32_t) _rtn);
  DefHeader( "XOR"   , NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _bitsxor);    Insert((int32_t) _rtn);
  DefHeader( "INVERT", NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _bitsnot);    Insert((int32_t) _rtn);
  //
  DefHeader( "!",      NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _store);      Insert((int32_t) _rtn);
  DefHeader( "@",      NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _fetch);      Insert((int32_t) _rtn);
  DefHeader( "+!",     NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _addstore);   Insert((int32_t) _rtn);
  DefHeader( "-!",     NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _substore);   Insert((int32_t) _rtn);
  DefHeader( "C!",     NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _storebyte);  Insert((int32_t) _rtn);
  DefHeader( "C@",     NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _fetchbyte);  Insert((int32_t) _rtn);
  DefHeader( "C@C!",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _ccopy);      Insert((int32_t) _rtn);
  DefHeader( "CMOVE",  NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _cmove);      Insert((int32_t) _rtn);
  DefHeader( "KEY",    NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _key);        Insert((int32_t) _rtn);
  DefHeader( "EMIT",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _emit);       Insert((int32_t) _rtn);
  DefHeader( "NUMBER", NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _number);     Insert((int32_t) _rtn);
  DefHeader( "FIND",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) _find);       Insert((int32_t) _rtn);
//  
#ifdef PICO
  DefHeader( "TEMPON",    NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_temp_on);           Insert((int32_t) _rtn);
  DefHeader( "TEMPOFF",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_temp_off);          Insert((int32_t) _rtn);
  DefHeader( "ADC_INIT",  NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_adc_init);          Insert((int32_t) _rtn);
  DefHeader( "ADC_GPIO",  NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_adc_gpio_init);     Insert((int32_t) _rtn);
  DefHeader( "ADC_INPUT", NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_adc_select_input);  Insert((int32_t) _rtn);
  DefHeader( "ADC_READ",  NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_adc_read);          Insert((int32_t) _rtn);
  //
  DefHeader( "IO_GET_DIR",  NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_get_dir);                      Insert((int32_t) _rtn);
  DefHeader( "IO_GET_MA",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_get_drive_strength);           Insert((int32_t) _rtn);
  DefHeader( "IO_GET_DT",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_get_slew_rate);                Insert((int32_t) _rtn);
  DefHeader( "IO_NO_PULL",  NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_disable_pulls);                Insert((int32_t) _rtn);
  DefHeader( "IO_PULLUP",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_pull_up);                      Insert((int32_t) _rtn);
  DefHeader( "IO_PULLDN",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_pull_down);                    Insert((int32_t) _rtn);
  DefHeader( "IO_PULLUP?",  NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_is_pulled_up);                 Insert((int32_t) _rtn);
  DefHeader( "IO_PULLDN?",  NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_is_pulled_down);               Insert((int32_t) _rtn);
  DefHeader( "IO_HYSTO?",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_is_input_hysteresis_enabled);  Insert((int32_t) _rtn);
  DefHeader( "IO_GET",      NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_get);                          Insert((int32_t) _rtn);
  DefHeader( "IO_INIT",     NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_init);                         Insert((int32_t) _rtn);
  DefHeader( "IO_SET_DIR",  NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_set_dir);                      Insert((int32_t) _rtn);
  DefHeader( "IO_PUT",      NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_put);                          Insert((int32_t) _rtn);
  DefHeader( "IO_SET_MA",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_set_drive_strength);           Insert((int32_t) _rtn);
  DefHeader( "IO_SET_DT",   NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_set_slew_rate);                Insert((int32_t) _rtn);
  DefHeader( "IO_HYSTO_ON", NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_gpio_set_input_hysteresis_enabled); Insert((int32_t) _rtn);
  //
  DefHeader( "FLASH_ERASE",    NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_flash_sector_erase);   Insert((int32_t) _rtn);
  DefHeader( "FLASH!",         NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_flash_store);          Insert((int32_t) _rtn);
  DefHeader( "FLASH_LIST",     NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_flash_page_list);      Insert((int32_t) _rtn);
  DefHeader( "FLASH_ID",       NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_flash_get_unique_id);  Insert((int32_t) _rtn);
  DefHeader( "FLASH_PATRN",    NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_page_pattern);         Insert((int32_t) _rtn);
  DefHeader( "FLASH_WIPE",     NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_WipeAllSectors);       Insert((int32_t) _rtn);
  DefHeader( "FLASH_GET_LINE", NADAZ, NADAZ); Insert((int32_t) _docode); Insert((int32_t) pico_Get_Page_Line);        Insert((int32_t) _rtn);
  //
#endif  
}
//
void BuildWordEntries(int32_t where){
  DefHeader( "EXIT",      NADAZ, NADAZ);      Insert((int32_t) _docode);    Insert((int32_t) _rtn);  
       
  DefHeader( "BUG0",      NADAZ, NADAZ);      Insert((int32_t) _docolon);   InsertWord("EXIT");

  DefHeader( "SWP0",      NADAZ, NADAZ);      Insert((int32_t) _docolon);   InsertWord("EXIT");  

  DefHeader( "SWP1",      NADAZ, NADAZ);      Insert((int32_t) _docolon);   InsertWord("EXIT");  

  DefHeader( "SWP",       NADAZ, NADAZ);      Insert((int32_t) _docolon);   InsertWord("SWP0");  
                                                                            InsertWord("SWP1");     
                                                                            InsertWord("EXIT");
                                                                            
  DefHeader( "BUG",       NADAZ, NADAZ);      Insert((int32_t) _docolon);   InsertWord("BUG0");       
                                                                            InsertWord("EXIT");

  DefHeader( "WORD_TEST", NADAZ, NADAZ);      Insert((int32_t) _docolon);   InsertWord("SWP");        
                                                                            InsertWord("BUG");  
                                                                            InsertWord("EXIT");
}
