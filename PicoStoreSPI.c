#include "Common.h"
#ifdef PICO
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "hardware/spi.h"
#include "PicoStoreSPI.h"
#include "StackPrimitives.h"
#include "Utilities.h"
//
//#define VERBOSE
//
void pico_flash_sector_erase(void){ //(Sector - ) erase one sector = 4096 bytes
  int32_t s = FLASH_SECTORS_AVAILABLE; 
  int32_t SectorNumber = PopP; 
  int32_t Offset = (FLASH_SECTORS_OFFSET + (SectorNumber * FLASH_SECTOR_SIZE));  
  //
  if( ((short)SectorNumber >= 0) && (SectorNumber < s) ){
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(Offset, FLASH_SECTOR_SIZE);
    restore_interrupts (ints);
  }else{
    PrintBuf("Sector Erase Failed, sector number out of range: 0,1,2,...");
    PrintNumber(s-1);
  }
}
//
#ifdef VERBOSE 
void show_buffer(const uint8_t* data, int32_t number){ // for testing
  int32_t i;
  char buf[8];
  for(i=0;i<number;i++){
    sprintf(buf,"%02X",data[i]);
    PrintBuf(buf);
    if (i % 16 == 15){
      PrintBuf("\n");
    }else{
      PrintBuf(" ");
    }     
  }
}
#endif
//
void pico_flash_store(void){ // (dataPtr Sector Page - )
  int32_t PageNumber = PopP;
  int32_t SectorNumber = PopP;
  int32_t data = PopP;
  char buf[80];
  //  
#ifdef VERBOSE  
  sprintf(buf,"\nStore at: SN=%d PN=%d\n",SectorNumber, PageNumber);
  PrintBuf(buf);  
#endif
  int32_t s = FLASH_SECTORS_AVAILABLE;
  int32_t p = PAGES_PER_SECTOR;
  int32_t Offset = (FLASH_SECTORS_OFFSET + (SectorNumber * FLASH_SECTOR_SIZE) + (PageNumber * FLASH_PAGE_SIZE) );
  if( ((short)SectorNumber >= 0) && (SectorNumber < s) ){                                     
    if( ((short)PageNumber >= 0) && (PageNumber < p) ){      
#ifdef VERBOSE      
      show_buffer((const uint8_t*)data, FLASH_PAGE_SIZE);
#endif
      uint32_t ints = save_and_disable_interrupts();
      flash_range_program(Offset, (const uint8_t*)data, FLASH_PAGE_SIZE);                                    
      restore_interrupts (ints); 
    }else{
      PrintBuf("Page Store Failed, page number out of range: 0,1,2,...");
      PrintNumber(p-1);
    }
  }else{
    PrintBuf("Sector?, sector number out of range: 0,1,2,...");
    PrintNumber(s-1);
  }
}
//
void pico_flash_page_list(void){ // (sector Page - )
  int32_t PageNumber = PopP;  
  int32_t SectorNumber = PopP; 
  char buf[80]; 
  //
#ifdef VERBOSE  
  sprintf(buf,"\n SectorNumber=%d PageNumber=%d\n",SectorNumber, PageNumber);
  PrintBuf(buf);
#endif
  //  
  int i, len = FLASH_PAGE_SIZE;
  char sbuf[8];
  //
  int32_t s = FLASH_SECTORS_AVAILABLE;
  int32_t p = PAGES_PER_SECTOR;
  const uint8_t* Offset = (const uint8_t*)(XIP_BASE + FLASH_SECTORS_OFFSET + 
                                          (SectorNumber * FLASH_SECTOR_SIZE) +
                                          (PageNumber * FLASH_PAGE_SIZE) );
  //                                        
  if( ((short)SectorNumber >= 0) && (SectorNumber < s) ){                                     
    if( ((short)PageNumber >= 0) && (PageNumber < p) ){  
      PrintBuf("\n");    
      for (i = 0; i < len; ++i) {
        sprintf(sbuf,"%02x", Offset[i]);
        PrintBuf(sbuf);
        if (i % 16 == 15){
          PrintBuf("\n");
        }else{
          PrintBuf(" ");
        } 
      }
    }else{
      PrintBuf("Page Read Failed, page number out of range: 0,1,2,...");
      PrintNumber(p-1);
    }
  }else{
    PrintBuf("Page Read Failed, sector number out of range: 0,1,2,...");
    PrintNumber(s-1);
  }
}
//
void pico_flash_get_unique_id(void){  // ( - idPtr)
  flash_get_unique_id((uint8_t*)M.PicoIDbuf);
#ifdef VERBOSE
  PrintBuf("\n");
  show_buffer(M.PicoIDbuf, 8);
#endif
  PushP =  (int32_t)M.PicoIDbuf;
}
//
// make page pattern to test writing flash
//
void pico_page_pattern(void){ // (Offset - PatternPtr)
  int32_t offset = PopP;    
  int32_t i;
#ifdef VERBOSE
  PrintBuf("\nPattern Inc Offset is: ");
  PrintNumber(offset);
  PrintBuf("\n");
#endif
  for(i=0;i<256;i++){
    M.Page_Buf[i] = (unsigned char)(0xFF & (i+offset));
  }
#ifdef VERBOSE
  show_buffer(M.Page_Buf, 256);
#endif
  PushP =  (int32_t)M.Page_Buf;
}
/*
void pico_flash_do_cmd(int32_t*, int32_t*, int32_t){
  flash_do_cmd(int32_t*, int32_t*, int32_t);
}
*/
//
void pico_WipeAllSectors(void){ // ( - )
  int32_t i;
  PrintBuf("\nWipe takes a few seconds.\n");
  for(i=0;i<FLASH_SECTORS_AVAILABLE;i++){
    PushP = i;
    pico_flash_sector_erase();
  }
}
//
void pico_Get_Page_Line(void){ // (line page sector - )destination M.line_buf[64]
  int32_t LineNumber = PopP;   
  int32_t PageNumber = PopP;  
  int32_t SectorNumber = PopP;   
}
//
void pico_Put_Page_Line(void){ // source M.line_buf[64]
  int32_t LineNumber = PopP;   
  int32_t PageNumber = PopP;  
  int32_t SectorNumber = PopP;   
}
//
#endif