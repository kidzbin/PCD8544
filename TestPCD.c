#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "PCD8544.h"

#define _DIN 1
#define _CLK 0
#define _DC  2
#define _RST 4
#define _CE  3
#define _CONTRAST  40

int main(void)
{
  time_t rawtime;
  struct tm * timeinfo;
  char LCDStr[12];
  int cnt;

  if(wiringPiSetup() == -1)
    return -1;

  LCDInit(_CLK, _DIN , _DC , _CE , _RST, _CONTRAST);

  LCDClear();

  cnt = 1;

  while(cnt) 
  {
    //LCDSample();
    time ( &rawtime );
    timeinfo = localtime ( &rawtime ); 

    if(timeinfo->tm_hour<12)
      sprintf(LCDStr,"%02d:%02d:%02d AM ",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
    else
      sprintf(LCDStr,"%02d:%02d:%02d PM",timeinfo->tm_hour-12,timeinfo->tm_min,timeinfo->tm_sec);

    gotoXY(0,0);
    LCDString(LCDStr);      

    delay(1000);
  }
}