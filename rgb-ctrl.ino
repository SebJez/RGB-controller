#include <RotaryEncoder.h>
#include "RgbLight.h"

RotaryEncoder encoder(3,4);
RgbLight strip(9,10,11)

ISR(PCINT2_vect)
{
    encoder.tick();
}

void setup()
{
    // interrupts for rotary encoder
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT19) | (1 << PCINT20);

    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop()
{

}
