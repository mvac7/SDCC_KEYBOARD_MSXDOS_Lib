/* =============================================================================
   Test INKEY() 
   Test SDCC Keyboard MSXROM Lib v1.0
   Version: 1.0
   Date: 24/06/2018
   Author: mvac7/303bcn
   Architecture: MSX
   Format: .COM (MSXDOS)
   Programming language: C (SDCC)
   WEB: 
   mail: mvac7303b@gmail.com

   Description:
     Test the INKEY function of MSXROM Keyboard library.
     
   History of versions:
     - v1.0 (24/06/2018)<

     
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVars.h"
#include "../include/msxBIOS.h"
#include "../include/msxDOS.h"

#include "../include/keyboard.h"
#include "../include/textmode.h"



#define  SYSTEM 0x0005  // MSX-DOS entry



// Function Declarations -------------------------------------------------------
void System(char code);

void test();

//void INPUT(char* string);

//char strlen(char* text);




// constants  ------------------------------------------------------------------
const char text01[] = "Test v1.0 (24/06/2018)";
const char text02[] = "Test SDCC Keyboard MSXDOS Lib v1.0";


// global variable definition --------------------------------------------------



// Functions -------------------------------------------------------------------


//
void main(void)
{
  CLS(); 
    
  test();
  
  PRINT("\n\nEND");
  
  KillBuffer();
    
  System(_TERM0);
}



void test()
{
  char bytevalue;
  boolean isContinue=true;
  //char string[32];
  
  LOCATE(0,0);
  PRINT(text01);
  LOCATE(0,1);
  PRINT(text02);
  
  LOCATE(0,4);
  PRINT("Test INKEY");
  PRINT("\nPress ESC to exit.");
  
  LOCATE(1,7);
  PRINT("Press a key:");
  
  
  while(isContinue)
  {
    LOCATE(13,7);
    bytevalue = INKEY();    
    LOCATE(14,7);
    PrintFNumber(bytevalue,' ',3);
    if (bytevalue==27) isContinue=false; 
  }

  
}



// call system functions 
// see MSX Assembly Page > MSX-DOS 2 function calls
// http://map.grauw.nl/resources/dos2_functioncalls.php
void System(char code)
{
code;
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   C,4(IX)
	call SYSTEM

	pop  IX
__endasm; 
}




/*
void INPUT(char* string)
{
  //char length = 32;
  byte aKey=0;
  byte size=0;
  
  KillBuffer();
  
  while(1)
  {
    aKey = INKEY();
    if (aKey==13) break;
  
    if (aKey==8 && size>0)//BS
    {
      size--;
      bchput(127); //del
      string[size]=32; 
    }

    if(aKey>31 && aKey<123 && size<32) //length
    {
      //if (aKey>96) aKey-=32; //uppercase
      string[size]=aKey;
      bchput(aKey);
      size++;
    }  
  }
  string[size]=0; //END mark \0
}
*/



/*char strlen(char* text)
{
  byte i = 0;
  while(*(text++)) i++;
  return i;
}*/
