#pragma once

#include <Arduino.h>
#include "ds_i_pin.h"
/*!
*   @class ArdDirect
*   @brief interface class providing ctrol over
*		standard arduino pin control
*/
class ArdDirect: public IPin
{
public:
	explicit ArdDirect(const int &pin);
	void setup(int mode) override;
	void analog_write(int value) override;
	void digital_write(int value) override;
	int analog_read() override;
	int digital_read() override;
	virtual ~ArdDirect() = default;
	
	ArdDirect(const ArdDirect&) 			= delete;
	ArdDirect(ArdDirect&&) 					= delete;
	ArdDirect& operator=(const ArdDirect&) 	= delete;
	ArdDirect& operator=(ArdDirect&&) 		= delete;
private:
	int   _pin;
};
