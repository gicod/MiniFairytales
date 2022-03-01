#include "ds_maglock.h"

MagnetLock::MagnetLock(const int &pin, const int &active_level):
	_hw_controller(new ArdDirect(pin)),
	_active_level(active_level)
{
	this->_hw_controller->setup(OUTPUT);
    this->lock();
}

MagnetLock::MagnetLock(
	PCF8575 *pcf8575,
	const int &pin,
	const int &activation_on):
	_hw_controller(new PCF8575Adapter(pcf8575, pin)),
	_active_level(activation_on)
{
	this->_hw_controller->setup(OUTPUT);
}

void MagnetLock::lock()
{
	this->_hw_controller->digital_write(this->_active_level);
}

void MagnetLock::unlock()
{
	this->_hw_controller->digital_write(!this->_active_level);
}

MagnetLock::~MagnetLock()
{
	if (this->_hw_controller)
		delete this->_hw_controller;
}