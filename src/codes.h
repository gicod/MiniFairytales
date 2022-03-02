#pragma once
#include "common.h"
#include "../lib/data/Code.h"

enum
{
    CODES_STAGE_NONE,
    CODES_STAGE_GAME,
    CODES_STAGE_DONE
} codes_stage;

SimpleLed *ledsCodes[codes_ns::LEDS_PINS_COUNT];
Codes *codes;

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

void code_onCorrect(int index)
{
    *console << "code_onCorrect: " << index;
    static bool leds_state[codes_ns::LEDS_PINS_COUNT] = {};
    static size_t count = 0;

    if (!leds_state[index])
    {
        leds_state[index] = true;
        count++;
    }
    
    if (count < codes_ns::LEDS_PINS_COUNT)
    {
        ledsCodes[count]->on();
    }
    ledsNumbers->clear();
    *console << "\tcount: " << count << endl;
}

void gerkonsCodes_onActivated(int index)
{
    if (codes_stage != CODES_STAGE_GAME)
        return;
    // *console << "gerkonsCodes_onActivated: " << index << endl;

    mqtt_manager->publish("/er/music/play", "21");
    ledsNumbers->set(index, WHITE);

    char index_str[8] = {""};
    itoa(index + 1, index_str, 10);
    int tmp = codes->checkIsCorrect(index_str);
    *console << "code number: " << index_str << endl;// << "\tcheckIsCorrect: " << tmp << endl;
    if (tmp == -1)
        return;
    if (tmp == -2)
    {
        *console << "code fail" << endl;
        ledsNumbers->clear();
        return;
    }
    code_onCorrect(tmp);
}

void codes_init()
{
    for (size_t i = 0; i < codes_ns::LEDS_PINS_COUNT; i++)
        ledsCodes[i] = new SimpleLed(&PCF_3, codes_ns::LEDS_PINS[i], HIGH);

    for (size_t i = 0; i < codes_ns::LEDS_PINS_COUNT; i++)
        ledsCodes[i]->off();
        
    codes = new Codes(codes_ns::CODES_COUNT, codes_ns::CODES);
    
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
    codes_onActivate();
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