#pragma once
#include "common.h"
#include "../lib/data/Hint.h"

enum
{
    HINTS_STAGE_NONE,
    HINTS_STAGE_GAME,
    HINTS_STAGE_DONE
} hints_stage;

ArdSensor *gerkonsHints[hints_ns::GERKONS_HINTS_PINS_COUNT];
SimpleLed *ledsHints[hints_ns::LEDS_PINS_COUNT];
// h_ ->hints
Timer *t_h_showHint;
Timer *t_h_delayLedOn;

size_t hints_count = 0;
Hint* hints;

void hint_debug()
{
    console->print(F("led state:"));
    for (size_t i = 0; i < hints_ns::LEDS_PINS_COUNT; i++)
    {
        console->print(F(" "));
        console->print(hints->getIndexHint(i));
    }
    console->print(F("\thints state: "));
    for (size_t i = 0; i < hints_ns::HINTS_COUNT; i++)
    {
        console->print(F(" "));
        console->print(hints->getState(i));
    }
    console->println(F(""));
}

void hint_newHint()
{
    console->print(F("hint_newHint"));
    if (hints->newHint())
        console->println(F("\tnew hint"));
    else
    {
        console->println(F("\tnot new hint"));
        hint_debug();
        return;
    }

    for (size_t i = 0; i < hints_ns::LEDS_PINS_COUNT; i++)
        if (hints->getIndexHint(i) == -1)
            ledsHints[i]->off();
        else
            ledsHints[i]->on();  
    hint_debug();
}

void hints_onActivate()
{
    console->println(F("hints: onActivated"));
    if (complexity == BASIC)
    {
        hint_newHint();
        hint_newHint();
    }
    
    hints_stage = HINTS_STAGE_GAME;
    strcpy(props_states[HINTS_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void hints_onFinish()
{
    console->println(F("hints: onFinish"));
    for (size_t i = 0; i < hints_ns::LEDS_PINS_COUNT; ++i)
        ledsHints[i]->off();      
    t_h_showHint->pause();
    hints_stage = HINTS_STAGE_DONE;
    strcpy(props_states[HINTS_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void hints_scb(char* topic, uint8_t* payload, unsigned int len)
{
    char* payload_p = reinterpret_cast<char*>(payload);
    payload[len] = '\0';

    if(strcmp(topic, "/er/hints/cmd") == 0) {  
        console->print(F("/er/hints/cmd -> "));
        console->println(payload_p);

        static size_t index;
        index = static_cast<size_t>(atoi(payload_p)) - 1;

        if(index >= 0)
        {
            if(hints->getIndexLed(index) >= 0)
                ledsHints[hints->getIndexLed(index)]->off(); 
            t_h_delayLedOn->launch(DELAY_1_SEC, 1, [](void*){
                console->println(F("t_h_delayLedOn_cb"));
                hints->isDone(index);
                hint_newHint();
            });
        }
        else
            console->println(F("wrong !!! incorrect message"));
    }
}

void hints_init()
{
    for (size_t i = 0; i < hints_ns::GERKONS_HINTS_PINS_COUNT; ++i)
    {
        gerkonsHints[i] = new ArdSensor(&PCF_4, hints_ns::GERKONS_HINTS_PINS[i], LOW, 30);
        gerkonsHints[i]->setCallbackOnActivatedTagged([](int i){
            int tmp = hints->getIndexHint(i);
            if (tmp == -1)
                return;
            console->print(F("gerkonsHints_onActivated: "));
            console->print(i);
            console->print(F("\t hint: "));
            console->println(hints->getIndexHint(i));

            t_h_showHint->launch(hints_ns::DELAY_HINT[current_language][(size_t)tmp], 1, [](void*){
                console->println(F("t_h_showHint_cb"));
            });

            char tmp_c[8] ={""};
            itoa((tmp + 25), tmp_c, 10);
            mqtt_manager->publish("/er/music/play", tmp_c);
        },i);
    }
    for (size_t i = 0; i < hints_ns::LEDS_PINS_COUNT; ++i)
        ledsHints[i] = new SimpleLed(&PCF_5, hints_ns::LEDS_PINS[i], HIGH);
    for (size_t i = 0; i < hints_ns::LEDS_PINS_COUNT; ++i)
        ledsHints[i]->off();
    
    hints = new Hint(hints_ns::HINTS_COUNT, hints_ns::LEDS_PINS_COUNT);

    t_h_showHint = new Timer();
    t_h_delayLedOn = new Timer();

    hints_stage = HINTS_STAGE_NONE;
    strcpy(props_states[HINTS_STATE_POS], MQTT_STRSTATUS_READY);
}

void hints_routine()
{
    t_h_showHint->routine();
    t_h_delayLedOn->routine();

    if (hints_stage != HINTS_STAGE_GAME)
        return;

    if (t_h_showHint->is_running()
    || t_h_delayLedOn->is_running())
        return;

    for (size_t i = 0; i < hints_ns::GERKONS_HINTS_PINS_COUNT; ++i)
        gerkonsHints[i]->check();
}