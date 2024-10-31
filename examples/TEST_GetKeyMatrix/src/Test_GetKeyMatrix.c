/* =============================================================================
   Test GetKeyMatrix
   Version: 1.2 (30/10/2024)
   Author: mvac7/303bcn
   Architecture: MSX
   Format:  .COM (MSX-DOS)
   Programming language: C (SDCC)

   Description:
     Test the GetKeyMatrix function of the SDCC Keyboard MSX-DOS library
     
   History of versions:
     - v1.2 (30/10/2024) update to SDCC (4.1.12) Z80 calling conventions
     - v1.1 (11/08/2019) improved pulsation control
     - v1.0 (14/07/2018)
     
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"
#include "../include/msxDOS.h"

#include "../include/keyboard_MSX.h"




#define  HALT __asm halt __endasm   //Z80 instruction > wait for the next interrupt


// VRAM address tables T1 Screen 0 TXT40
#define BASE0 0x0000 // Name Table
#define BASE2 0x0800 // Pattern Table

//VDP Ports  
#define VDPVRAM   0x98  //VRAM Data (Read/Write)
#define VDPSTATUS 0x99  //VDP Status Registers



// Function Declarations -------------------------------------------------------
void System(char code);

char PEEK(unsigned int address);
void POKE(char value, unsigned int address);

void test(void);

void printKey(char column, char line);
void clearKey(char column, char line);

void PRINT(char column, char line, char* text);

void SCREEN0(void);
void SCREEN1(void);
char VPEEK(uint address);
void VPOKE(char value, unsigned int vaddr);
void SetVRAMtoWRITE(unsigned int vaddr);
void FastVPOKE(char value);


// constants  ------------------------------------------------------------------
const char text01[] = "Test GetKeyMatrix())";
const char text02[] = "Test SDCC Keyboard MSXDOS Lib";
const char text03[] = "Hold down ESC key to exit to DOS";



// TEST Apps
// map size width:40 height:24
// Size= 960
const char keyb_map[]={
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x20,0x20,0x20,0x20,0x20,0x37,0x20,0x20,0x20,0x36,0x20,0x20,0x20,0x35,0x20,0x20,
0x20,0x34,0x20,0x20,0x20,0x33,0x20,0x20,0x20,0x32,0x20,0x20,0x20,0x31,0x20,0x20,
0x20,0x30,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x18,0x17,0x17,0x17,0x12,
0x17,0x17,0x17,0x12,0x17,0x17,0x17,0x12,0x17,0x17,0x17,0x12,0x17,0x17,0x17,0x12,
0x17,0x17,0x17,0x12,0x17,0x17,0x17,0x12,0x17,0x17,0x17,0x19,0x20,0x20,0x20,0x20,
0x20,0x20,0x30,0x16,0x20,0x37,0x20,0x16,0x20,0x36,0x20,0x16,0x20,0x35,0x20,0x16,
0x20,0x34,0x20,0x16,0x20,0x33,0x20,0x16,0x20,0x32,0x20,0x16,0x20,0x31,0x20,0x16,
0x20,0x30,0x20,0x16,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x14,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x13,0x20,0x20,0x20,0x20,
0x20,0x20,0x31,0x16,0x20,0x3B,0x20,0x16,0x20,0x5D,0x20,0x16,0x20,0x5B,0x20,0x16,
0x20,0x5C,0x20,0x16,0x20,0x3D,0x20,0x16,0x20,0x2D,0x20,0x16,0x20,0x39,0x20,0x16,
0x20,0x38,0x20,0x16,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x14,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x13,0x20,0x20,0x20,0x20,
0x20,0x20,0x32,0x16,0x20,0x42,0x20,0x16,0x20,0x41,0x20,0x16,0x41,0x63,0x4F,0x16,
0x20,0x2F,0x20,0x16,0x20,0x2E,0x20,0x16,0x20,0x2C,0x20,0x16,0x41,0x63,0x43,0x16,
0x20,0x27,0x20,0x16,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x14,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x13,0x20,0x20,0x20,0x20,
0x20,0x20,0x33,0x16,0x20,0x4A,0x20,0x16,0x20,0x49,0x20,0x16,0x20,0x48,0x20,0x16,
0x20,0x47,0x20,0x16,0x20,0x46,0x20,0x16,0x20,0x45,0x20,0x16,0x20,0x44,0x20,0x16,
0x20,0x43,0x20,0x16,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x14,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x13,0x20,0x20,0x20,0x20,
0x20,0x20,0x34,0x16,0x20,0x52,0x20,0x16,0x20,0x51,0x20,0x16,0x20,0x50,0x20,0x16,
0x20,0x4F,0x20,0x16,0x20,0x4E,0x20,0x16,0x20,0x4D,0x20,0x16,0x20,0x4C,0x20,0x16,
0x20,0x4B,0x20,0x16,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x14,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x13,0x20,0x20,0x20,0x20,
0x20,0x20,0x35,0x16,0x20,0x5A,0x20,0x16,0x20,0x59,0x20,0x16,0x20,0x58,0x20,0x16,
0x20,0x57,0x20,0x16,0x20,0x56,0x20,0x16,0x20,0x55,0x20,0x16,0x20,0x54,0x20,0x16,
0x20,0x53,0x20,0x16,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x14,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x13,0x20,0x20,0x20,0x20,
0x20,0x20,0x36,0x16,0x46,0x33,0x20,0x16,0x46,0x32,0x20,0x16,0x46,0x31,0x20,0x16,
0x43,0x4F,0x44,0x16,0x43,0x41,0x50,0x16,0x47,0x52,0x41,0x16,0x43,0x54,0x52,0x16,
0x53,0x48,0x49,0x16,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x14,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x13,0x20,0x20,0x20,0x20,
0x20,0x20,0x37,0x16,0x52,0x45,0x54,0x16,0x53,0x45,0x4C,0x16,0x42,0x53,0x20,0x16,
0x53,0x54,0x4F,0x16,0x54,0x41,0x42,0x16,0x45,0x53,0x43,0x16,0x46,0x35,0x20,0x16,
0x46,0x34,0x20,0x16,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x14,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,
0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x15,0x17,0x17,0x17,0x13,0x20,0x20,0x20,0x20,
0x20,0x20,0x38,0x16,0x52,0x49,0x47,0x16,0x44,0x4F,0x57,0x16,0x55,0x50,0x20,0x16,
0x4C,0x45,0x46,0x16,0x44,0x45,0x4C,0x16,0x49,0x4E,0x53,0x16,0x48,0x4F,0x4D,0x16,
0x53,0x50,0x41,0x16,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x1A,0x17,0x17,0x17,0x11,
0x17,0x17,0x17,0x11,0x17,0x17,0x17,0x11,0x17,0x17,0x17,0x11,0x17,0x17,0x17,0x11,
0x17,0x17,0x17,0x11,0x17,0x17,0x17,0x11,0x17,0x17,0x17,0x1B,0x20,0x20,0x20,0x20};


// global variable definition --------------------------------------------------



// Functions -------------------------------------------------------------------


//
void main(void)
{
//  char colorInk=0;
//  char colorBG=0;
//  char colorBDR=0;
  char scrcolumns=0;
  
//  colorInk=PEEK(FORCLR);
//  colorBG=PEEK(BAKCLR);
//  colorBDR=PEEK(BDRCLR);
  scrcolumns=PEEK(LINLEN);
     
  test();
 
  KillBuffer();


//EXIT MSXDOS ------------------------------------------------------------------
  //put the screen as it was.
//  COLOR(colorInk,colorBG,colorBDR);   //restore colors

  POKE(scrcolumns,LINLEN);            //restore screen columns
  
  if(scrcolumns<33) SCREEN1();        //restore screen mode
  else SCREEN0();
    
  System(DOS_TERM0);                  //return to DOS
//--------------------------------------------------------------------- end EXIT  

}



// call system functions 
// see MSX Assembly Page > MSX-DOS 2 function calls
// http://map.grauw.nl/resources/dos2_functioncalls.php
void System(char code) __naked
{
code;	//A
__asm
  ld   C,A
  jp BDOS
__endasm; 
}




/* =============================================================================
   PEEK
 
   Function : Read a 8 bit value from the RAM.
   Input    : [unsigned int] RAM address
   Output   : [char] value
============================================================================= */
char PEEK(unsigned int address) __naked
{
address;	//HL
__asm

  ld   A,(HL)

  ret
__endasm;
}



void POKE(char value, unsigned int address) __naked
{
value;   //A
address; //DE
__asm 
  ld   (DE),A
  ret
__endasm;
}



/* -----------------------------------------------------------------------------

----------------------------------------------------------------------------- */
void test(void)
{
  char val;
  char keyPressed;
  char row;
  uint i;
  uint offset;
  uint vaddr = BASE2 + (32*8);
  char isExit = 0;
  
//  COLOR(LIGHT_GREEN,DARK_GREEN,DARK_GREEN);    
  SCREEN0();

  //CopyToVRAM((uint) keyb_map,BASE0,0x3c0);
  SetVRAMtoWRITE(BASE0);
  for(i=0;i<0x3c0;i++){FastVPOKE(keyb_map[i]);}
    
  PRINT(0,0,text01);
  PRINT(0,1,text02);
  PRINT(0,2,text03);
  
  
  // genera una copia de los caracteres en la posicion 128 en negativo
  for(i=0;i<768;i++)
  {
    val = ~ VPEEK(vaddr); //invierte el valor
    VPOKE(val,vaddr+768); //lo copia en un tile superior
    vaddr++;              // avanza a la siguiente posicion de la vram
  }

  


  while(isExit<60)  //bucle infinito
  {
    HALT;

    if (!(GetKeyMatrix(7)&Bit2)) isExit++;
    else isExit=0;

    for(row=0;row<9;row++)
    {
      keyPressed = GetKeyMatrix(row);
      
      if (keyPressed==255)
      {
        //No keys have been pressed on this row 
        offset = 244+(row*80);
		SetVRAMtoWRITE(BASE0 + offset);
		for(i=0;i<32;i++){FastVPOKE(keyb_map[offset++]);}
        
      }else{
        if (!(keyPressed&Bit0)) printKey(7,row); else clearKey(7,row); 
        if (!(keyPressed&Bit1)) printKey(6,row); else clearKey(6,row); 
        if (!(keyPressed&Bit2)) printKey(5,row); else clearKey(5,row); 
        if (!(keyPressed&Bit3)) printKey(4,row); else clearKey(4,row); 
        if (!(keyPressed&Bit4)) printKey(3,row); else clearKey(3,row); 
        if (!(keyPressed&Bit5)) printKey(2,row); else clearKey(2,row); 
        if (!(keyPressed&Bit6)) printKey(1,row); else clearKey(1,row); 
        if (!(keyPressed&Bit7)) printKey(0,row); else clearKey(0,row); 
      }
    
    } //END For
  } //END while
    

}



/* -----------------------------------------------------------------------------

----------------------------------------------------------------------------- */
void printKey(char column, char line)
{ 
  char B;
  uint addr;
//  uint addr;
  
  addr = BASE0 + 244 + (column*4) +(line*80);
//  addr= (uint) keyb_map + vaddr;
  
  SetVRAMtoWRITE(addr);
  for(B=0;B<3;B++){FastVPOKE(keyb_map[addr++]+96);}
  
//  FastVPOKE(keyb_map[addr++]+96);
//  FastVPOKE(keyb_map[addr++]+96);
//  FastVPOKE(keyb_map[addr]+96);
  
/*  tmpTile = PEEK(addr++)+96;
  VPOKE(tmpTile,vaddr++);
  tmpTile = PEEK(addr++)+96;
  VPOKE(tmpTile,vaddr++);
  tmpTile = PEEK(addr)+96;
  VPOKE(tmpTile,vaddr);*/
}


void clearKey(char column, char line)
{ 
  char B;
  uint addr;
//  uint addr;
  
  addr = BASE0 + 244 + (column*4) +(line*80);
//  addr= (uint) keyb_map + vaddr;
  
  SetVRAMtoWRITE(addr);
  for(B=0;B<3;B++){FastVPOKE(keyb_map[addr++]);}
  
//  FastVPOKE(keyb_map[addr++]+96);
//  FastVPOKE(keyb_map[addr++]+96);
//  FastVPOKE(keyb_map[addr]+96);
  
/*  tmpTile = PEEK(addr++)+96;
  VPOKE(tmpTile,vaddr++);
  tmpTile = PEEK(addr++)+96;
  VPOKE(tmpTile,vaddr++);
  tmpTile = PEEK(addr)+96;
  VPOKE(tmpTile,vaddr);*/
}



/* =============================================================================
 Display a string to screen
============================================================================= */
void PRINT(char column, char line, char* text)
{
  char character;
  uint vaddr = BASE0 + (line*40)+column; //<------ BASE0 for screen 0; use BASE5 for screen 1 

  while(*(text))
  {
    character=*(text++);
    VPOKE(character,vaddr++);
  }
}




/* =============================================================================
 SCREEN0
 
 Description: 
           Initialice TEXT 1 (40 columns) or TEXT 2 (80 columns) screen mode.
		   
           Note: 
             To set the T2 mode, you must first set 80 columns with the WIDTH
			 function (only MSX computers with V9938 and BIOS that supports
			 this mode).
 Input:    -
 Output:   -
============================================================================= */
void SCREEN0(void) __naked
{
__asm
  push IX
  
  ld   A,(#LINLEN)
  ld   (#LINL40),A   ;copy columns seting with WIDTH to LINL40 system var
   
  ld   IX,#BIOS_INITXT
$CallBIOS:
  ld   IY,(#EXPTBL-1)
  call BIOS_CALSLT
  
  ei    
  pop  IX
  ret  
__endasm;
}



/* =============================================================================
 SCREEN1
 
 Description: 
           Initialice GRAPHIC 1 screen mode (32 columns x 24 lines).

 Input:    -
 Output:   -
============================================================================= */
void SCREEN1(void) __naked
{
__asm
  push IX
  
  ld   A,(#LINLEN)   ;get a last value set with WIDTH function 
  ld   (#LINL32),A   ;set system variable
   
  ld   IX,#BIOS_INIT32
  jp   $CallBIOS  
__endasm;
}



/* =============================================================================
	VPEEK
	Description:	Reads a value from video RAM. 
	Input:			[unsigned int] VRAM address
	Output:			[char] value
============================================================================= */
char VPEEK(uint vaddr) __naked
{
vaddr;
__asm
  push IX

  ld   IX,#BIOS_RDVRM
  ld   IY,(#EXPTBL-1)
  call BIOS_CALSLT
  ei

  pop  IX
  ret
__endasm;
}



/* =============================================================================
	VPOKE
	Description:	Writes a value to the video RAM. 
					Warning!! The order of the values has been reversed to 
					optimize the function taking advantage of the new 
					Z80 calling conventions.
	Input: 			[char] value
					[unsigned int] VRAM address
	Output:			- 
============================================================================= */
void VPOKE(char value, unsigned int vaddr) __naked
{
value;    //A
vaddr;  //DE
__asm
  push IX

  ex   DE,HL
  ld   IX,#BIOS_WRTVRM
  ld   IY,(#EXPTBL-1)
  call BIOS_CALSLT
  ei

  pop  IX
  ret
__endasm;
}




/* =============================================================================
 SetVRAMtoWRITE
 Description: Enable VDP to write (Similar to BIOS SETWRT)
 Input      : [char] VRAM address
 Output     : -             
============================================================================= */
void SetVRAMtoWRITE(unsigned int vaddr) __naked
{
vaddr;	//HL
__asm
  push IX

  ld   IX,#BIOS_SETWRT
  ld   IY,(#EXPTBL-1)
  call BIOS_CALSLT
  ei

  pop  IX
  ret
__endasm;
}   



/*===============================================================================
 FastVPOKE                                
 Function :	Writes a value to the next video RAM position. 
				Requires the VDP to be in write mode, either by previously 
				using VPOKE or SetVRAMtoWRITE functions.
 Input    : A - value
===============================================================================*/
void FastVPOKE(char value) __naked
{
value;	//A
__asm
  out  (VDPVRAM),A  
  ret
__endasm;
}