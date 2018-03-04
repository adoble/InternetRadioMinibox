/*
 *
 * Author: Andrew Doble
 * Creation date: 21.01.2018
 */

#ifndef _EPDecoder2to3_H
#define _EPDecoder2to3_H

#include <Arduino.h>

#include "VirtualPin.h"


class VirtualPinDecoder : public VirtualPin
{
private:
uint8_t _nPins;
uint8_t * _controllerPins;
uint8_t * _patternValues;

void construct(uint8_t nPins, uint8_t controllerPins[], uint8_t patternValues[]);


public:

VirtualPinDecoder(uint8_t nPins, uint8_t controllerPins[], uint8_t patternValues[]);

VirtualPinDecoder(uint8_t controllerPinA, uint8_t controllerPinB, uint8_t patternValue1, uint8_t patternValue2);

void begin();
void write(uint8_t value);
void mode(uint8_t mode);


};

#endif
