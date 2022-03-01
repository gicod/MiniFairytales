#include "ds_ard_direct.h"

ArdDirect::ArdDirect(const int &pin):
	_pin(pin)
{
}

void ArdDirect::setup(int mode)
{
    switch (mode) {
    case OUTPUT:
        pinMode(this->_pin, OUTPUT);
        ArdDirect::digital_write(LOW);
        break;
    case INPUT:
        ::pinMode(this->_pin, INPUT);
        break;
    case INPUT_PULLUP: ///> INPUT_PULLUP is a default state
    default:
        ::pinMode(this->_pin, INPUT_PULLUP);
    }
}

void ArdDirect::analog_write(int value)
{
    ::analogWrite(this->_pin, value);
}

void ArdDirect::digital_write(int value)
{
    ::digitalWrite(this->_pin, value);
}

int ArdDirect::analog_read()
{
    return ::analogRead(this->_pin);
}

int ArdDirect::digital_read()
{
    return ::digitalRead(this->_pin);
}