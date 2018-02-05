/*
 * client:
   -----

   VirtualPin vsChipSelect = VirtualPin (12,13, HIGH, HIGH, LOW);   // i.e. pins, pattern
   VirtualPin memChipSelect = VirtualPin(12,13, HIGH, LOW, LOW);
   VirtualPin fpChipSelect = VirtualPin(12,13, LOW, HIGH, LOW);

   //Also
   // controllerPins = {12, 13};
   // patternValues = {LOW, HIGH};
   // VirtualPin fpChipSelect = VirtualPin(2,controllerPins, patternValues, LOW);

   Lemon_VS1053 player = Lemon_VS1053(XRST, vsChipSelect, XDCS, DREQ);

   vsChipSelect.digitalWrite(LOW)   output --> H, H, L
   vsChipSelect.digitalWrite(HIGH)  output --> H, H, H

   memChipSelect.digitalWrite(LOW)   output --> H, L, H
   memChipSelect.digitalWrite(HIGH)  output --> H, H, H

   fpChipSelect.digitalWrite(LOW)   output --> L, H, H
   fpChipSelect.digitalWrite(HIGH)  output --> H, H, H

   fpChipSelect.digitalWrite(LOW)   output --> L, H, H
   memChipSelect.digitalWrite(HIGH)  error!


   in library:
   Lemon_VS1053::Lemon_VS1053(XRST, XCS, XDCS, DREQ) {
    ....
    virtualChipSelectPin = defaultVirualChipSelectPin;
   }

   Lemon_VS1053::Lemon_VS1053(XRST, vsChipSelect, XDCS, DREQ) {
    ...
    virtualChipSelectPin= vsChipSelect;
   }

   chipSelector.digitalWrite(LOW); ...
 *
 * Author: Andrew Doble
 * Creation date: 21.01.2018
 */

#include "VirtualPinDecoder.h"

/** General constructor for an n pin to (2**n)-1 output pins decoder
 */
VirtualPinDecoder::VirtualPinDecoder(uint8_t nPins, uint8_t controllerPins[], uint8_t patternValues[])
{
        construct(nPins, controllerPins, patternValues);
}

/**
 * Convenience constructor for a 2 pin to 3 pin decoder.
 *
 * param:
 *   unit8_t controllerPinA Input pin
 *   unit8_t controllerPinB Input pin
 *   unit8_t patternValue1  Input pin A pattern value
 *   unit8_t patternValue2  Input pin B pattern value
 *
 */
VirtualPinDecoder::VirtualPinDecoder(uint8_t controllerPinA, uint8_t controllerPinB, uint8_t patternValue1, uint8_t patternValue2)
{
        int nPins = 2;
        uint8_t * controllerPins;
        uint8_t * patternValues;

        controllerPins = new uint8_t[nPins];
        patternValues = new uint8_t[nPins];

        controllerPins[0] = controllerPinA;
        controllerPins[1] = controllerPinB;
        patternValues[0] = patternValue1;
        patternValues[1] = patternValue2;

        construct(nPins, controllerPins, patternValues);

}


void VirtualPinDecoder::construct(uint8_t nPins, uint8_t controllerPins[], uint8_t patternValues[])
{
        _nPins = nPins;
        _controllerPins = controllerPins;
        _patternValues = patternValues;

        for (int i = 0; i < _nPins; i++) {
                pinMode(controllerPins[i], OUTPUT);
        }
}

/**
 * Set pins to HIGH
 */
void VirtualPinDecoder::begin()
{
        for (int i= 0; i < _nPins; i++) {
                digitalWrite(_controllerPins[i], HIGH);
        }
}

void VirtualPinDecoder::write(uint8_t value)
{
        if (value == LOW) {
                for (int i= 0; i < _nPins; i++) {
                        digitalWrite(_controllerPins[i], _patternValues[i]);
                }
        } else {
                for (int i= 0; i < _nPins; i++) {
                        digitalWrite(_controllerPins[i], HIGH);
                }
        }
}

void VirtualPinDecoder::mode(uint8_t mode) {
  // Not required
}
