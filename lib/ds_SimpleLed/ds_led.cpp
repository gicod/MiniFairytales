#include "ds_led.h"

SimpleLed::SimpleLed(const int &pin, const int &active_level):
	_hw_controller(new ArdDirect(pin)),
	_active_level(active_level)
{
	this->_hw_controller->setup(OUTPUT);
}

SimpleLed::SimpleLed(
	PCF8575 *pcf8575,
	const int &pin,
	const int &activation_on):
	_hw_controller(new PCF8575Adapter(pcf8575, pin)),
	_active_level(activation_on)
{
	this->_hw_controller->setup(OUTPUT);
}

void SimpleLed::on()
{
	this->_hw_controller->digital_write(this->_active_level);
}

void SimpleLed::off()
{
	this->_hw_controller->digital_write(!this->_active_level);
}

SimpleLed::~SimpleLed()
{
	if (this->_hw_controller)
		delete this->_hw_controller;
}
