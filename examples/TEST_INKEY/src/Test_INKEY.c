/* =============================================================================
	Test INKEY from Keyboard MSX-DOS Lib v1.1
	Version: 1.1 (8/12/2023)
	Author: mvac7/303bcn
	Architecture: MSX
	Format: .COM (MSXDOS)
	Programming language: C and Z80 assembler
	Compiler: SDCC 4.1.12 or newer

	Description:
	 Test the INKEY function of MSXROM Keyboard library.
	 
	History of versions:
	- v1.1 (8/12/2023)
	- v1.0 (24/06/2018)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"
#include "../include/msxDOS.h"

#include "../include/keyboard_MSX.h"
#include "../include/textmode_MSX.h"





// Function Declarations -------------------------------------------------------
void test(void);

void PrintFill(char length, char character);

char PEEK(uint address);





// constants  ------------------------------------------------------------------
const char text01[] = "Test Keyboard MSXDOS Lib";



// global variable definition --------------------------------------------------



// Functions -------------------------------------------------------------------


//
void main(void)
{
  CLS(); 
    
  test();
  
  PRINT("\n\nEND");
}



void test(void)
{
	char bytevalue=0;
	char columns = PEEK(LINLEN);

	LOCATE(0,0);
	PRINT(text01);
	
	LOCATE(0,1);
	PrintFill(columns,'-');

	LOCATE(0,4);
	PrintLN("Test INKEY");
	PRINT("Press ESC to exit.");

	LOCATE(1,7);
	PRINT("Press a key:");


	while(bytevalue!=27)
	{
		LOCATE(13,7);
		bytevalue = INKEY();    
		LOCATE(14,7);
		PrintFNumber(bytevalue,' ',3);
		PRINT("         ");
		LOCATE(18,7);
		if(bytevalue==127) PRINT("[Del]");
		else if(bytevalue>31)
		{
		   PRINT("[");
		   bchput(bytevalue);
		   PRINT("]");		
		}else{
			switch(bytevalue)
			{
				case 8:
					PRINT("[BS]");
					break;
					
				case 9:
					PRINT("[TAB]");
					break;
					
				case 11:
					PRINT("[Home]");
					break;
					
				case 13:
					PRINT("[Enter]");
					break;
					
				case 18:
					PRINT("[Ins]");
					break;
				
				case 24:
					PRINT("[Select]");
					break;
				
				case 27:
					PRINT("[Esc]");
					break;
					
				case 28:
					PRINT("[Right]");
					break;
					
				case 29:
					PRINT("[Left]");
					break;
					
				case 30:
					PRINT("[Up]");
					break;
					
				case 31:
					PRINT("[Down]");
					break;
					
				default:
					break;	
			}
			
		}

		KillBuffer();
	}
  
}



/*
Returns a string with a variable length, all containing either the same character, 
which is defined as an ASCII code or the first character of a string.
https://www.msx.org/wiki/STRING$()
*/
void PrintFill(char length, char character) __naked
{
length;		//A
character;	//L
__asm
  push IX

  ld   B,A	;length
  ld   A,L	;character
  
  ld   IX,#BIOS_CHPUT
  ld   IY,(EXPTBL-1)
  
$PCHAR_LOOP:
  call BIOS_CALSLT
  djnz $PCHAR_LOOP
  
  ei  
  pop  IX
  ret
__endasm;
}



char PEEK(uint address) __naked
{
address;
__asm
  ld   A,(HL)
  ret
__endasm;
}

