#pragma once
#include "common.h"

using namespace digits_ns;

enum
{
    DIGITS_STAGE_NONE,
    DIGITS_STAGE_GAME,
    DIGITS_STAGE_DONE
} digits_stage;

Lstrip *ledsDigits;
Timer *t_digitsShow;
Timer *t_ledsDigitsOff;
Timer *t_ledsDigitsNextShow;

void t_digitsShow_cb(void*);
void digits_onActivate()
{
    console->println(F("digits: onActivated"));
    t_digitsShow->launch(1,1);
    // t_ledsDigitsNextShow->launch(DELAY_LEDS_NEXT_SHOW, Timer::TIMER_LOOP, [](void*){
    //     *console << "t_ledsDigitsNextShow_cb" << endl;
    // });
    digits_stage = DIGITS_STAGE_GAME;
    strcpy(props_states[DIGITS_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void digits_onFinish()
{
    console->println(F("digits: onFinish"));
    digits_stage = DIGITS_STAGE_DONE;
    strcpy(props_states[DIGITS_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void t_digitsShow_cb(void*)
{
    // *console << "t_digitsShow" << endl;
    static bool state = false;
    state = !state;
    static size_t count = 0;
    ledsDigits->clear();       
        
    if (state)
    {
        for (size_t i = 0; i < digits_ns::LSTRIPS_COUNT; i++)
            if (NUMBERS_STATE_LEDS[count][i])
                ledsDigits->set(i, CYAN, false);
        ledsDigits->show();           
        t_digitsShow->launch(DELAY_LEDS_OFF, 2);
    }    
    else
    {
        count++;
        t_digitsShow->launch(DELAY_LEDS_ON, 2);
    }

    if (count >= NUMBERS_COUNT)
    {
        count = 0;
        t_digitsShow->launch(DELAY_LEDS_NEXT_SHOW, 2);
    }
}

void digits_init()
{
    ledsDigits = new Lstrip(digits_ns::LSTRIPS_PIN, digits_ns::LSTRIPS_COUNT, digits_ns::RGB_ORDER);
    ledsDigits->set_segmentation(digits_ns::LSTRIPS_COUNT);
    // ledsDigits->set_brightness(11);
    t_digitsShow = new Timer(t_digitsShow_cb);
    t_ledsDigitsOff = new Timer();
    t_ledsDigitsNextShow = new Timer();

    digits_stage = DIGITS_STAGE_NONE;
    strcpy(props_states[DIGITS_STATE_POS], MQTT_STRSTATUS_READY);

    //debug
    // digits_onActivate();
}

void digits_routine()
{
    t_digitsShow->routine();
    t_ledsDigitsOff->routine();
    t_ledsDigitsNextShow->routine();

    if (digits_stage != DIGITS_STAGE_GAME)
        return;
}