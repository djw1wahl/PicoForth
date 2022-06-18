#include "Common.h"
#include <Arduino.h>
//
char buf[256];
//
extern "C" { int32_t forth(); }
//
void establishContact() {
  strcpy(buf, "Pico_Forth Version: 0.00\n");
  strcat(buf, __DATE__);
  strcat(buf, "  ");
  strcat(buf, __TIME__);
  strcat(buf, "\nDesigned by James Luscher & Daniel Wahl, <- The good guys!\n");
  strcat(buf, "Terminal Settings: 115200 Baud, 8N1, No Local Echo, Send <cr>. \n\n");
  //
  while (Serial.available() <= 0) { } // wait for key press
  Serial.read();
}
//
void setup() { 
  Serial.begin(115200);   
  while (!Serial) { }
  establishContact();   
  Serial.write(buf);
  Serial.flush();
  //
  forth(); // run the forth virtual machine
  //
  Notify("Forth has bit the bullet.\n")
}
//
#ifdef __cplusplus
extern "C" {
#endif
  //
  // compiled under C not C++
  //
  void serial_putchar(char c){ Serial.print(c); }
  //
  int32_t serial_getchar(void){ 
    while (Serial.available() <= 0) { }         // block
    int32_t thekey = Serial.read() & 0xFF ;     // non blocking, returns -1 if no chars
    return(thekey);
  }
  //
  void PrintBuf(char* buf){ for(int32_t i=0;i<strlen(buf);i++){ Serial.print(buf[i]); } }
  void PrintNumber(int32_t n){ char abuf[16]; sprintf(abuf," %8.8X ", n); PrintBuf(abuf); }
  //
#ifdef __cplusplus  
}
#endif
//
void loop(void){ delay(100); } // all the magic is up in setup()
//
