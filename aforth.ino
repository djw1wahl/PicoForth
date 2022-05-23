#include <Arduino.h>
#include "Common.h"
//
char Title[128];
char Communicate[128];
//
extern "C" { int32_t forth(); }
//
void establishContact() {
  strcpy(Title, "Pico_Forth Version: 0.00\n");
  strcat(Title, __DATE__);
  strcat(Title, "  ");
  strcat(Title, __TIME__);
  strcat(Title, "\nDesigned by James Luscher & Daniel Wahl, <- The good guys!\n");
  strcpy(Communicate, "Terminal Settings: 115200 Baud, 8N1, No Local Echo, Send <cr>. \n\n");
  //
  while (Serial.available() <= 0) { } // wait for key press
  Serial.read();
}
//
void setup() { 
  Serial.begin(115200);   
  while (!Serial) { }
  establishContact();   
  Serial.write(Title);
  Serial.write(Communicate);
  Serial.flush();
  //
  forth(); // run the forth virtual machine
  //
  strcpy(Title, "Forth has bit the bullet.\n"); 
  PrintBuf(Title);
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
  //
#ifdef __cplusplus  
}
#endif
//
void loop(){ } // all the magic is up in setup()
//
