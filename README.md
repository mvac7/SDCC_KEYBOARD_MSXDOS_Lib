# SDCC Keyboard MSX-DOS Functions Library (object type)

Version: 1.0

Date: 2/03/2016

Author: mvac7/303bcn

Architecture: MSX

Format: C Object (SDCC .rel)

Programming language: C

WEB:
 
mail: mvac7303b@gmail.com




## 1. Introduction

This project is an Open Source library with basic functions for reading the 
keyboard of MSX computers.

Use them for developing MSX applications using Small Device C Compiler (SDCC).

This library uses functions from the MSX BIOS through interslot calls (CALSLT),
so that they can be used in MSXDOS applications.

Includes an application for test and learning purposes.



## 2. Acknowledgments
  
Thanks for Info & help, to:

* Avelino Herrera > http://msx.atlantes.org/index_es.html
* Nerlaska > http://albertodehoyonebot.blogspot.com.es
* Fubu > http://www.gamerachan.org/fubu/
* Marq/Lieves!Tuore > http://www.kameli.net/lt/
* Sapphire/Z80ST > http://z80st.auic.es/
* Pentacour > http://pentacour.com/
* JamQue/TPM > http://www.thepetsmode.com/
* Andrear > http://andrear.altervista.org/home/msxsoftware.php
* Konamiman > https://www.konamiman.com
* MSX Assembly Page > http://map.grauw.nl/resources/msxbios.php
* Portar MSX Tech Doc > http://nocash.emubase.de/portar.htm
* MSX Resource Center > http://www.msx.org/
* Karoshi MSX Community > http://karoshi.auic.es/
* BlueMSX >> http://www.bluemsx.com/
* OpenMSX >> http://openmsx.sourceforge.net/
* Meisei  >> ?



## 3. Requirements

* Small Device C Compiler (SDCC) v3.6 http://sdcc.sourceforge.net/
* Hex2bin v2.2 http://hex2bin.sourceforge.net/ 



## 4. Functions

* void KillBuffer() - Clear keyboard buffer.
* char INKEY() - One character input (waiting) and return its code.
* char GetKeyMatrix(line) - Returns the value of the specified line from the keyboard matrix.


## 5. Keyboard Matrix

  Info by MSX Assembly Page 
  http://map.grauw.nl/articles/keymatrix.php


### 5.1 International
```  
bit  7     6     5     4     3     2     1     0
----------------------------------------------------
  0  7 &   6 ^   5 %   4 $   3 #   2 @   1 !   0 )
  1  ; :   ] }   [ {   \ |   = +   - _   9 (   8 *
  2  B     A     acent / ?   . >   , <   `     ' "
  3  J     I     H     G     F     E     D     C
  4  R     Q     P     O     N     M     L     K
  5  Z     Y     X     W     V     U     T     S
  6  F3    F2    F1    CODE  CAPS  GRAPH CTRL  SHIFT
  7  RET   SEL   BS    STOP  TAB   ESC   F5    F4
  8  Right Down  Up    Left  DEL   INS   HOME  SPACE
  9  NUM4  NUM3  NUM2  NUM1  NUM0  NUM/  NUM+  NUM*
 10  NUM.  NUM,  NUM-  NUM9  NUM8  NUM7  NUM6  NUM5
``` 