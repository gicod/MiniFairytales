#pragma once
#include "common.h"

enum
{
    HINTS_STAGE_NONE,
    HINTS_STAGE_GAME,
    HINTS_STAGE_DONE
} hints_stage;

ArdSensor *gerkonsHints[hints_ns::GERKONS_HINTS_PINS_COUNT];
SimpleLed *ledsHints[hints_ns::LEDS_PINS_COUNT];
Timer *t_80;
Timer *t_81;
Timer *t_82;
Timer *t_83;
Timer *t_84;
Timer *t_85;
Timer *t_86;

void hints_onActivate()
{
    console->println(F("hints: onActivated"));
    hints_stage = HINTS_STAGE_GAME;
    strcpy(props_states[HINTS_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void hints_onFinish()
{
    console->println(F("hints: onFinish"));
    hints_stage = HINTS_STAGE_DONE;
    strcpy(props_states[HINTS_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void hints_init()
{
    for (size_t i = 0; i < hints_ns::GERKONS_HINTS_PINS_COUNT; ++i)
    {
        gerkonsHints[i] = new ArdSensor(&PCF_4, hints_ns::GERKONS_HINTS_PINS[i], LOW, 30);
        gerkonsHints[i]->setCallbackOnActivatedTagged([](int i){
            *console << "gerkonsHints_onActivated: " << i << endl;
            ledsHints[i]->on();
            delay(300);
            ledsHints[i]->off();
        },i);
        ledsHints[i] = new SimpleLed(&PCF_5, hints_ns::LEDS_PINS[i], HIGH);
    }
    for (size_t i = 0; i < hints_ns::GERKONS_HINTS_PINS_COUNT; ++i)
        ledsHints[i]->off();

    t_80 = new Timer();
    t_81 = new Timer();
    t_82 = new Timer();
    t_83 = new Timer();
    t_84 = new Timer();
    t_85 = new Timer();
    t_86 = new Timer();

    hints_stage = HINTS_STAGE_NONE;
    strcpy(props_states[HINTS_STATE_POS], MQTT_STRSTATUS_READY);
}

void hints_routine()
{
    t_80->routine();
    t_81->routine();
    t_82->routine();
    t_83->routine();
    t_84->routine();
    t_85->routine();
    t_86->routine();

    if (hints_stage != HINTS_STAGE_GAME)
        return;

    for (size_t i = 0; i < hints_ns::GERKONS_HINTS_PINS_COUNT; ++i)
        gerkonsHints[i]->check();
}