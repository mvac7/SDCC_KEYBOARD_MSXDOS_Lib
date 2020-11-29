/* =============================================================================
   SDCC Keyboard MSX-DOS Functions Library (object type)
   Version: 1.0
   Date: 2 March 2016
   Author: mvac7
   Architecture: MSX
   Format: C Object (SDCC .rel)
   Programming language: C
   WEB: 
   mail: mvac7303b@gmail.com

   Description:
     Functions for reading the keyboard of MSX computers.
     Executes BIOS functions via inter-slot call (CALSLT)
     
============================================================================= */
#include "../include/keyboard.h"

#include "../include/msxSystemVars.h"
#include "../include/msxBIOS.h"



/* =============================================================================
   KillBuffer
 
  Function : Clear keyboard buffer
  Input    : -
  Output   : -
============================================================================= */
void KillBuffer() __naked
{
__asm   
  push IX
   
  ld   IX,#KILBUF
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
    
  pop  IX
  ret  
__endasm;
}



/* =============================================================================
   INKEY
  
   Function : One character input (waiting) and return its code
   Input    : -
   Output   : [char] key code
============================================================================= */
char INKEY() __naked
{
__asm   
   push IX
  
   ld   IX,#CHGET
   ld   IY,(EXPTBL-1)
   call CALSLT
   ei
   
   ld   L,A

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
row;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
     
  ld   A,4(IX)
  
  ld   IX,#SNSMAT
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
  
  ld   L,A
  
  pop  IX
  ret
__endasm;
}