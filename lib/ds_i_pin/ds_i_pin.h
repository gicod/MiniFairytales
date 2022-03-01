#pragma once

#include <Arduino.h>
/*!
*   @class IPinOut
*   @brief interface class providing an abstruction level
*          between arduino IOpins and various devices (leds, locks and etc)
*/
class IPin
{
public:
	IPin();
	virtual void setup(int mode) = 0; // todo: rename to pin_mode?
	virtual void analog_write(int value) = 0;
	virtual void digital_write(int value) = 0;
	virtual int analog_read() = 0;
	virtual int digital_read() = 0;
	virtual ~IPin() = default;
};
