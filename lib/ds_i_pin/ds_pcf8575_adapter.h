#pragma once
#include <ds_i_pin.h>
#include <PCF8575.h> // https://github.com/RobTillaart/PCF8575

class PCF8575Adapter: public IPin {
public:
    explicit PCF8575Adapter(PCF8575 *pcf8575, int pin);
	void setup(int) override;
	void analog_write(int value) override;
	void digital_write(int value) override;
	int analog_read() override;
	int digital_read() override;
	virtual ~PCF8575Adapter() = default;
private:
    PCF8575 *_pcf8575;
    int _pin;
};