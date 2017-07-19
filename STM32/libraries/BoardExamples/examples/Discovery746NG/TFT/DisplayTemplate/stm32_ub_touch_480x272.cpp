// driver found at
// http://mikrocontroller.bplaced.net/wordpress/?page_id=5329
// datasheet:
// https://www.newhavendisplay.com/appnotes/datasheets/touchpanel/FT5336.pdf
//--------------------------------------------------------------
// File     : stm32_ub_touch_480x272.c
// Datum    : 15.07.2015
// Version  : 1.1
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F746
// IDE      : OpenSTM32
// GCC      : 4.9 2015q2
// Module   : CubeHAL, STM32_UB_I2C3
// Funktion : Touch-Funktionen
//            Touch-Controller (Chip = FT5336GQQ)
//
// Hinweis  : Settings :
//            I2C-Slave-ADR = [0x70]
//            FRQ-Max = 400 kHz
//            I2C-3 [SCL=PH7, SDA=PH8]
//            Interrupt-Pin = PI13 (wird nicht benutzt)
//
//  im H-File einstellen ob Single- oder Multitouch
//  benutzt werden soll
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_touch_480x272.h"
//#include "stm32_ub_i2c3.h"

MultiTouch_Data_t MultiTouch_Data;
Touch_Data_t Touch_Data; 

//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
uint8_t P_Touch_ReadID(void);
uint8_t P_Touch_GetContacts(void);
uint8_t P_Touch_GetPositions(void);

//--------------------------------------------------------------
uint8_t FT5336_TOUCH_REG[5][4]; // fuer Register Adressen


//--------------------------------------------------------------
// Init vom Touch
// Return_wert :
//  -> ERROR   , wenn Touch nicht gefunden wurde
//  -> SUCCESS , wenn Touch OK
//-------------------------------------------------------------- 
ErrorStatus UB_Touch_Init(void)
{
  uint8_t touch_id=0,n; 

  Touch_Data.status=TOUCH_RELEASED;
  Touch_Data.xp=0;
  Touch_Data.yp=0;  

  MultiTouch_Data.cnt=0;
  for(n=0;n<TOUCH_MAX_CONTACT;n++) {
    MultiTouch_Data.p[n].xp=0;
    MultiTouch_Data.p[n].yp=0;
  }  

  // init der Touch Register Adressen
  FT5336_TOUCH_REG[0][0]=FT5336_P1_XL_REG;
  FT5336_TOUCH_REG[0][1]=FT5336_P1_XH_REG;
  FT5336_TOUCH_REG[0][2]=FT5336_P1_YL_REG;
  FT5336_TOUCH_REG[0][3]=FT5336_P1_YH_REG;

  FT5336_TOUCH_REG[1][0]=FT5336_P2_XL_REG;
  FT5336_TOUCH_REG[1][1]=FT5336_P2_XH_REG;
  FT5336_TOUCH_REG[1][2]=FT5336_P2_YL_REG;
  FT5336_TOUCH_REG[1][3]=FT5336_P2_YH_REG;

  FT5336_TOUCH_REG[2][0]=FT5336_P3_XL_REG;
  FT5336_TOUCH_REG[2][1]=FT5336_P3_XH_REG;
  FT5336_TOUCH_REG[2][2]=FT5336_P3_YL_REG;
  FT5336_TOUCH_REG[2][3]=FT5336_P3_YH_REG;

  FT5336_TOUCH_REG[3][0]=FT5336_P4_XL_REG;
  FT5336_TOUCH_REG[3][1]=FT5336_P4_XH_REG;
  FT5336_TOUCH_REG[3][2]=FT5336_P4_YL_REG;
  FT5336_TOUCH_REG[3][3]=FT5336_P4_YH_REG;

  FT5336_TOUCH_REG[4][0]=FT5336_P5_XL_REG;
  FT5336_TOUCH_REG[4][1]=FT5336_P5_XH_REG;
  FT5336_TOUCH_REG[4][2]=FT5336_P5_YL_REG;
  FT5336_TOUCH_REG[4][3]=FT5336_P5_YH_REG;

  // init vom I2C
//  UB_I2C3_Init();

  // kleine Pause
  HAL_Delay(TOUCH_INIT_DELAY);    
  
  touch_id=P_Touch_ReadID();
  if(touch_id!=FT5336_ID) return ERROR;  

  return(SUCCESS);
}


//--------------------------------------------------------------
// auslesen vom Touch-Status und der Touch-Koordinaten
//
// es kann entweder der Single-Touch "Touch_Data"
// oder der Multi-Touch "MultiTouch_Data" benutzt werden 
//
// Return_wert :
//  -> ERROR   , wenn Touch nicht gefunden wurde
//  -> SUCCESS , wenn Touch OK
//
// Touch_Data.status : [TOUCH_PRESSED, TOUCH_RELEASED]
// Touch_Data.xp     : [0...479]
// Touch_Data.yp     : [0...271] 
//
// MultiTouch_Data.cnt            : [0...5]
// MultiTouch_Data.p[0..4].xp     : [0...479]
// MultiTouch_Data.p[0..4].yp     : [0...271] 
//-------------------------------------------------------------- 
ErrorStatus UB_Touch_Read(void)
{
  uint8_t check,n;

  // init
  Touch_Data.status=TOUCH_RELEASED;
  Touch_Data.xp=0;
  Touch_Data.yp=0;

  MultiTouch_Data.cnt=0;
  for(n=0;n<TOUCH_MAX_CONTACT;n++) {
    MultiTouch_Data.p[n].xp=0;
    MultiTouch_Data.p[n].yp=0;
  }

  check=P_Touch_GetContacts();
  if(check>TOUCH_MAX_CONTACT) return ERROR;

  if(check>0) {
    // wenn touch betaetigt
    MultiTouch_Data.cnt=check;
    Touch_Data.status=TOUCH_PRESSED;
    check=P_Touch_GetPositions();
    if(check>TOUCH_MAX_CONTACT) {
      // fehler beim lesen
      Touch_Data.status=TOUCH_RELEASED;
      Touch_Data.xp=0;
      Touch_Data.yp=0;

      MultiTouch_Data.cnt=0;
      for(n=0;n<TOUCH_MAX_CONTACT;n++) {
        MultiTouch_Data.p[n].xp=0;
        MultiTouch_Data.p[n].yp=0;
      }
      return ERROR;
    }    
    // erste koordinate fuer single touch speichern
    Touch_Data.xp=MultiTouch_Data.p[0].xp;
    Touch_Data.yp=MultiTouch_Data.p[0].yp;
  }

  return(SUCCESS);
}

//--------------------------------------------------------------
// interne Funktion
// ID auslesen (mehrmals versuchen)
// bei Fehler -> return = 0
//--------------------------------------------------------------
uint8_t P_Touch_ReadID(void)
{  
  int16_t i2c_wert;
  uint8_t n;

  for(n=0;n<5;n++) {
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, FT5336_ID_REG);
    if(i2c_wert>0) {
      if((i2c_wert&0xFF)==FT5336_ID) return(FT5336_ID);
    }
  }  

  return 0;
}

//--------------------------------------------------------------
// interne Funktion
// anzahl der Touch Kontakte ermitteln
// ret_wert : 0...5 = Anzahl der Kontakte
//               99 = Error
//--------------------------------------------------------------
uint8_t P_Touch_GetContacts(void)
{
  uint8_t ret_wert=0;
  int16_t i2c_wert;  
  
  i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, FT5336_STATUS_REG);
  if(i2c_wert<0) return 99;

  // wert maskieren
  ret_wert = (uint8_t)(i2c_wert & FT5336_STATUS_MASK);

  if(ret_wert>TOUCH_MAX_CONTACT) return 0;

  #if USE_MULTITOUCH==0
    // bei Singletouch maximal ein Kontakt
    if(ret_wert>1) ret_wert=1;
  #endif

  return(ret_wert);
}

//--------------------------------------------------------------
// interne Funktion
// die Touch positionen von allen Kontakten ermitteln
// ret_wert :     0 = OK
//               99 = Error
//--------------------------------------------------------------
// interne Funktion
uint8_t P_Touch_GetPositions(void)
{
  uint8_t n,adr,wert_lo,wert_hi;
  int16_t i2c_wert;
  uint16_t position;

  if(MultiTouch_Data.cnt==0) return 0;

  // alle daten einlesen
  for(n=0;n<MultiTouch_Data.cnt;n++) {
    // x_lo
    adr=FT5336_TOUCH_REG[n][0];
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
    if(i2c_wert<0) return 99;
    wert_lo = i2c_wert & FT5336_LO_MASK;
    // x_hi
    adr=FT5336_TOUCH_REG[n][1];
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
    if(i2c_wert<0) return 99;
    wert_hi = i2c_wert & FT5336_HI_MASK;    
    position=(wert_hi<<8)|wert_lo;
    if(position>=TOUCH_MAXY) position=(TOUCH_MAXY-1);
    MultiTouch_Data.p[n].yp=position;
    // y_lo
    adr=FT5336_TOUCH_REG[n][2];
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
    if(i2c_wert<0) return 99;
    wert_lo = i2c_wert & FT5336_LO_MASK;
    // y_hi
    adr=FT5336_TOUCH_REG[n][3];
    i2c_wert=UB_I2C3_ReadByte(FT5336_I2C_ADDR, adr);
    if(i2c_wert<0) return 99;
    wert_hi = i2c_wert & FT5336_HI_MASK;    
    position=(wert_hi<<8)|wert_lo;
    if(position>=TOUCH_MAXX) position=(TOUCH_MAXX-1);
    MultiTouch_Data.p[n].xp=position;
  }

  return 0;
}
