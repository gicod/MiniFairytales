#pragma once
#include "common.h"

enum
{
    COLOR_RINGS_STAGE_NONE,
    COLOR_RINGS_STAGE_GAME,
    COLOR_RINGS_STAGE_DONE
} color_rings_stage;

ArdSensor *btnLeft, *btnRight;
SimpleLed *ledsBtns;
Lstrip *ledsRings;

Color colors_current[color_rings_ns::SEGMENT_COUNT];
uint8_t circle_index = 0;
bool led_state = true;

constexpr size_t CIRCLE_CENTERS[color_rings_ns::CIRCLE_COUNT] = {4,6,8,14,16,21};

void color_rings_onActivate()
{
    console->println(F("color_rings: onActivated"));
    color_rings_stage = COLOR_RINGS_STAGE_GAME;
    strcpy(props_states[COLOR_RINGS_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void color_rings_onFinish()
{
    console->println(F("color_rings: onFinish"));
    color_rings_stage = COLOR_RINGS_STAGE_DONE;
    strcpy(props_states[COLOR_RINGS_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void color_rings_init()
{
    btnLeft = new ArdSensor(&PCF, color_rings_ns::BTN_LEFT_PIN, LOW, 50);
        btnLeft->setCallbackOnActivated([](){
            *console << "btnLeft_onActivated" << endl;
            ledsBtns->on();
            *console << "led on" << endl;
        });
    btnRight = new ArdSensor(&PCF, color_rings_ns::BTN_RIGHT_PIN, LOW, 50);
        btnRight->setCallbackOnActivated([](){
            *console << "btnRight_onActivated" << endl;
            ledsBtns->off();
            *console << "led off" << endl;
        });
    ledsBtns = new SimpleLed(&PCF, color_rings_ns::LED_BTNS_PIN, HIGH);
    // delay(50);
    // ledsBtns->off();
    // *console << "led off" << endl;

    color_rings_stage = COLOR_RINGS_STAGE_NONE;
    strcpy(props_states[COLOR_RINGS_STATE_POS], MQTT_STRSTATUS_READY);
}

void color_rings_routine()
{
    // btnLeft->check();
    // btnRight->check();
}