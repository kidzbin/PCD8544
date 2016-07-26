
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include "PCD8544.h"

#define  DEBUG 0

// LCD pins  
// LCD1 - GND
// LCD2 - VCC
// LCD3 - CLK
// LCD4 - Din
// LCD5 - D/C
// LCD6 - CS 
// LCD7 - RST
// LCD8 - LED

typedef struct 
{
	uint8_t CLK;
	uint8_t DIN;
	uint8_t DC;
	uint8_t CE;
	uint8_t RST;
	uint8_t CONTRAST;
}_LCDCtrl;

_LCDCtrl LCDCtrl;

void gotoXY(uint8_t x, uint8_t y)
{
  SendLCD( 0, 0x80 | x);  // Column.
  SendLCD( 0, 0x40 | y);  // Row.
}

void LCDInit(uint8_t CLK, uint8_t DIN , uint8_t DC , uint8_t CE ,uint8_t RST,uint8_t CONTRAST)
{
	LCDCtrl.CLK      = CLK;
	LCDCtrl.DIN      = DIN;
	LCDCtrl.DC       = DC;
	LCDCtrl.CE       = CE;
	LCDCtrl.RST      = RST;
	LCDCtrl.CONTRAST = CONTRAST;

	pinMode(LCDCtrl.CLK , OUTPUT);
	pinMode(LCDCtrl.DIN , OUTPUT);
	pinMode(LCDCtrl.DC  , OUTPUT);
	pinMode(LCDCtrl.CE  , OUTPUT);
	pinMode(LCDCtrl.RST , OUTPUT);
#if 0
 // Reset the controller state...
    digitalWrite(LCDCtrl.RST, HIGH);
    digitalWrite(LCDCtrl.CE, HIGH);
    digitalWrite(LCDCtrl.RST, LOW);
    delay(100);
    digitalWrite(LCDCtrl.RST, HIGH);

    // Set the LCD parameters...
    SendLCD(CMD, 0x21);  // extended instruction set control (H=1)
    SendLCD(CMD, 0x13);  // bias system (1:48)

//    if (this->model == CHIP_ST7576) {
//       SendLCD(CMD, 0xe0);  // higher Vop, too faint at default
//       SendLCD(CMD, 0x05);  // partial display mode
//   } else {
    SendLCD(CMD, 0xc2);  // default Vop (3.06 + 66 * 0.06 = 7V)
 //   }

    SendLCD(CMD, 0x20);  // extended instruction set control (H=0)
    SendLCD(CMD, 0x09);  // all display segments on

    // Clear RAM contents...
    LCDClear();

    // Activate LCD...
    SendLCD(CMD, 0x08);  // display blank
    SendLCD(CMD, 0x0c);  // normal mode (0x0d = inverse mode)
    delay(100);

    // Place the cursor at the origin...
    SendLCD(CMD, 0x80);
    SendLCD(CMD, 0x40);
#else
	digitalWrite(LCDCtrl.CE , LOW);
	digitalWrite(LCDCtrl.RST, LOW);
	digitalWrite(LCDCtrl.RST, HIGH);
	digitalWrite(LCDCtrl.CE , HIGH);

	SendLCD(CMD,0x21);
	SendLCD(CMD,0xB2);
	SendLCD(CMD,0x04);
	SendLCD(CMD,0x14);
	SendLCD(CMD,0x0C);

  	SendLCD(LOW, 0x20);
  	SendLCD(LOW, 0x0C);
#endif

    //Setup Contrast
    SendLCD(CMD, 0x21);  // extended instruction set control (H=1)
    SendLCD(CMD, 0x80 | (LCDCtrl.CONTRAST & 0x7f));
    SendLCD(CMD, 0x20);  // extended instruction set control (H=0)

  	LCDClear();
}

void LCDClear(void)
{
	uint16_t i;
	for ( i = 0; i < LCDWIDTH*LCDHEIGHT/8 ; i++)
		SendLCD(DATA,0x00);

}

void SendLCD(uint8_t bMode,uint8_t bData)
{
	uint8_t i;

	digitalWrite(LCDCtrl.DC , bMode);
	digitalWrite(LCDCtrl.CE , LOW);
	//shiftOut(LCDCtrl.DIN,LCDCtrl.CLK, MSBFIRST, bData) ;
	
	i=0x08;
	do
	{
		i--;
		// printf("%d ",(bData>>i)&0x01);
		digitalWrite(LCDCtrl.DIN , (bData>>i)&0x01 );
		delayMicroseconds(5);
		digitalWrite(LCDCtrl.CLK , HIGH);
		delayMicroseconds(5);
		digitalWrite(LCDCtrl.CLK , LOW );
		delayMicroseconds(5);

	}while(i);	


	digitalWrite(LCDCtrl.CE , HIGH);

#if DEBUG
	printf("%2x ",bData);

	i=0x08;
	do
	{
		i--;
		printf("%d ",(bData>>i)&0x01);
	}while(i);

	printf("\n");
#endif
}

void LCDCharacter(char character)
{
	uint8_t index;
  	SendLCD(DATA, 0x00);
  	for (index = 0; index < 5; index++)
  	{
    	SendLCD(DATA, ASCII[character - 0x20][index]);
  	}
  	SendLCD(DATA, 0x00);
}

void LCDString(char *characters)
{
	while (*characters)
	{
	  LCDCharacter(*characters++);
	}
}

void LCDSample(void)
{
	LCDClear();
 	gotoXY(7,1);
  	LCDString("Nokia 5110");
  	gotoXY(4,2);
  	LCDString("Scroll Demo");
}