/* =============================================================================
	Keyboard MSX-DOS Library (fR3eL Project)
	Version: 1.1 (8/12/2023)
	Author: mvac7/303bcn [mvac7303b@gmail.com]
	Architecture: MSX
	Format: C object (SDCC .rel)
	Programming language: C and Z80 assembler
	Compiler: SDCC 4.1.12 or newer   

	Description:
     Functions for reading the keyboard of MSX computers.
     Executes BIOS functions via inter-slot call (CALSLT)
	 
	History of versions:
	- v1.1 ( 8/12/2023) update to SDCC (4.1.12) Z80 calling conventions
	- v1.0 ( 2/ 3/2016)     
============================================================================= */
#include "../include/keyboard_MSX.h"

#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"



/* =============================================================================
	KillBuffer

	Function : Clear keyboard buffer
	Input    : -
	Output   : -
============================================================================= */
void KillBuffer(void) __naked
{
__asm   
  push IX
   
  ld   IX,#BIOS_KILBUF
  ld   IY,(EXPTBL-1)
  call BIOS_CALSLT
  ei
    
  pop  IX
  ret  
__endasm;
}



/* =============================================================================
	INKEY

	Function : Waits for a key press and returns its value
	Input    : -
	Output   : [char] key code
============================================================================= */
char INKEY(void) __naked
{
__asm   
   push IX
  
   ld   IX,#BIOS_CHGET
   ld   IY,(EXPTBL-1)
   call BIOS_CALSLT
   ei

   pop  IX
   ret
__endasm;
}




/* =============================================================================
	GetKeyMatrix

	Function : Returns the value of the specified line from the keyboard matrix.
			   Each line provides the status of 8 keys.
			   To know which keys correspond, you will need documentation that 
			   includes a keyboard table.
	Input    : [char] row 
	Output   : [char] state of the keys. 1 = not pressed; 0 = pressed
============================================================================= */
char GetKeyMatrix(char row) __naked
{
row;	// A
__asm
  push IX
  
  ld   IX,#BIOS_SNSMAT
  ld   IY,(EXPTBL-1)
  call BIOS_CALSLT
  ei
  
  pop  IX
  ret
__endasm;
}