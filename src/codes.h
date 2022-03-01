#pragma once
#include "common.h"

enum
{
    CODES_STAGE_NONE,
    CODES_STAGE_GAME,
    CODES_STAGE_DONE
} codes_stage;

SimpleLed *ledsCodes[codes_ns::LEDS_PINS_COUNT];

Timer *t_70;
Timer *t_71;
Timer *t_72;
Timer *t_73;
Timer *t_74;
Timer *t_75;
Timer *t_76;
Timer *t_77;
Timer *t_78;
Timer *t_79;

void codes_onActivate()
{
    console->println(F("codes: onActivated"));
    codes_stage = CODES_STAGE_GAME;
    strcpy(props_states[CODES_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void codes_onFinish()
{
    console->println(F("codes: onFinish"));
    codes_stage = CODES_STAGE_DONE;
    strcpy(props_states[CODES_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void gerkonsCodes_onActivated(int index)
{
    if (codes_stage != CODES_STAGE_GAME)
        return;
    *console << "gerkonsCodes_onActivated: " << index << endl;

    //debug
    static int i = 0;
    *console << "i = " << i << endl;
    ledsCodes[i]->on();
    delay(500);
    ledsCodes[i]->off();
    i++;
    if (i == codes_ns::LEDS_PINS_COUNT)
    {
        i = 0;
        // for (size_t i = 0; i < codes_ns::LEDS_PINS_COUNT; i++)
        //     ledsCodes[i]->off();
    }
    
}

void codes_init()
{
    for (size_t i = 0; i < codes_ns::LEDS_PINS_COUNT; i++)
        ledsCodes[i] = new SimpleLed(&PCF_3, codes_ns::LEDS_PINS[i], HIGH);

    for (size_t i = 0; i < codes_ns::LEDS_PINS_COUNT; i++)
        ledsCodes[i]->off();
    
    t_70 = new Timer();
    t_71 = new Timer();
    t_72 = new Timer();
    t_73 = new Timer();
    t_74 = new Timer();
    t_75 = new Timer();
    t_76 = new Timer();
    t_77 = new Timer();
    t_78 = new Timer();
    t_79 = new Timer();

    codes_stage = CODES_STAGE_NONE;
    strcpy(props_states[CODES_STATE_POS], MQTT_STRSTATUS_READY);

    ///debug
}

void codes_routine()
{
    t_70->routine();
    t_71->routine();
    t_72->routine();
    t_73->routine();
    t_74->routine();
    t_75->routine();
    t_76->routine();
    t_77->routine();
    t_78->routine();
    t_79->routine();

    if (codes_stage != CODES_STAGE_GAME)
        return;
}