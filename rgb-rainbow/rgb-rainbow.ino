#include "RgbLight.h"
#include "../RgbLight.h"
RgbLight strip(10,9,11);

void setup()
{

}
void loop()
{
  for(int h=0; h<100; h+=1) 
    {
      strip.setHsv(h/100.0,1,0.33,true);
      delay(100);
    }
    /*strip.setColor(255,0,0);
    delay(1000);
    strip.setColor(0,255,0);
    delay(1000);
    strip.setColor(0,255,255);
    delay(1000);*/
}
