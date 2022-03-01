#pragma once
#include "common.h"

ArdSensor *gerkons_PCF_2[devices_ns::PCF_2_GERKONS_PINS_COUNT]; // rainbow, codes
Lstrip *ledsNumbers; // rainbow, codes

extern void gerkonsRainbow_onActivated(int i);
extern void gerkonsCodes_onActivated(int i);

void devices_init()
{
    for (size_t i = 0; i < devices_ns::PCF_2_GERKONS_PINS_COUNT; ++i)
    {
        gerkons_PCF_2[i] = new ArdSensor(&PCF_2, devices_ns::PCF_2_GERKONS_PINS[i], LOW, 50);
        gerkons_PCF_2[i]->setCallbackOnActivatedTagged([](int i){
            // *console << "PCF_2_gerkons_onActivated: " << i << endl;
            gerkonsRainbow_onActivated(i);
            gerkonsCodes_onActivated(i);
        }, i);
    }

    ledsNumbers = new Lstrip(devices_ns::LSTRIPS_NUMBERS_PIN, devices_ns::LSTRIPS_NUMBERS_COUNT, devices_ns::RGB_ORDER);
    ledsNumbers->set_segmentation(devices_ns::LSTRIPS_NUMBERS_IN_EACH_SEGM_COUNT, devices_ns::LSTRIPS_NUMBERS_IN_EACH_SEGM);
}

void devices_routine()
{
    for (auto& gerkons : gerkons_PCF_2)
        gerkons->check();
}