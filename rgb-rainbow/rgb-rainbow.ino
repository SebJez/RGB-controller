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

}
