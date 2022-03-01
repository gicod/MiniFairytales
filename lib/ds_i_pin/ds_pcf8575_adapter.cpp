#include "ds_pcf8575_adapter.h"

PCF8575Adapter::PCF8575Adapter(PCF8575 *pcf8575, int pin):
    _pcf8575(pcf8575), _pin(pin)
{   
}

void PCF8575Adapter::setup(int)
{
    this->_pcf8575->begin();
}

void PCF8575Adapter::analog_write(int value)
{
    this->digital_write(value); // no analog_write support
}

void PCF8575Adapter::digital_write(int value)
{
    this->_pcf8575->write(this->_pin, value);
}

int PCF8575Adapter::analog_read()
{
    return this->analog_read();
}

int PCF8575Adapter::digital_read()
{
    return static_cast<int>(this->_pcf8575->read(this->_pin));
}