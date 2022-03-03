#pragma once
#include "common.h"
#include "../lib/data/Code.h"
#include "../lib/data/CodeWithError.h"

enum
{
    CODES_STAGE_NONE,
    CODES_STAGE_GAME,
    CODES_STAGE_DONE
} codes_stage;

SimpleLed *ledsCodes[codes_ns::LEDS_PINS_COUNT];
Codes *codes;
CodeWithError *code16, *code17;
//c_ -> codes
Timer *t_c_codesCheck;
Timer *t_c_blinkRed;
Timer *t_c_blinkGreen;
Timer *t_c_delayCodeFail;
Timer *t_c_digitsShow;

void codes_onActivate()
{
    console->println(F("codes: onActivated"));
    codes_stage = CODES_STAGE_GAME;
    strcpy(props_states[CODES_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void codes_onFinish()
{
    console->println(F("codes: onFinish"));
    mqtt_manager->publish("/er/music/play", codes_ns::SOUND_FINISH);    
    t_c_digitsShow->launch(1,1);
    codes_stage = CODES_STAGE_DONE;
    strcpy(props_states[CODES_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void code_onCorrect(int _index)
{
    static size_t index;
    index = _index;
    static size_t count = 0;
    count++;
    *console << "code_onCorrect: " << index << "\tcount: " << count << endl;
    ledsCodes[count]->on();

    static size_t blink_count;
    blink_count = 0;
    mqtt_manager->publish("/er/music/play", codes_ns::SOUND_FAIL);
    t_c_blinkGreen->launch(300, 6, [](void*){
        // *console << "t_c_blinkGreen_cb" << endl;
        blink_count++;
        if (blink_count % 2)
            ledsNumbers->set(GREEN);
        else
            ledsNumbers->clear();

        if (blink_count == 6)
        {
            // *console << "t_c_blinkGreen_cb end" << endl;
            char index_str[8] = {""};
            itoa(index + 1, index_str, 10);
            mqtt_manager->publish("/er/hints/cmd", index_str);
            if (index == 2)
                mqtt_manager->publish("/er/coil/cmd", "finish");
            else if (index == 9)
                mqtt_manager->publish("/er/color_rings/cmd", "finish");
            else if (index == 14)
                mqtt_manager->publish("/er/digits/cmd", "finish");
        }
    });

    if (count == codes_ns::CORRECT_CODES_COUNT[complexity])
    {
        codes_onFinish();
    }
}

void codes_onFail(int _index)
{
    *console << "codes_onFail: " << _index << endl;
    static int index;
    index = _index;
    static bool state;
    state = false;
    mqtt_manager->publish("/er/music/play", codes_ns::SOUND_FAIL);
    t_c_blinkRed->launch(300, 6, [](void*){
        // *console << "t_c_blinkRed_cb" << endl;
        state = !state;
        if (state)
            ledsNumbers->set(RED);
        else
            ledsNumbers->clear();

        if (index == -15)
            t_c_delayCodeFail->launch(DELAY_2_SEC, 1, [](void*){
                *console << "t_c_delayCodeFail_cb" << endl;
                mqtt_manager->publish("/er/music/play", codes_ns::SOUND_FAIL_16_PAS);
            });
        else if (index == -16)
            t_c_delayCodeFail->launch(DELAY_2_SEC, 1, [](void*){
                *console << "t_c_delayCodeFail_cb" << endl;
                mqtt_manager->publish("/er/music/play", codes_ns::SOUND_FAIL_17_PAS);
            });
    });
}

void codes_check(int index)
{
    *console << "codes_check: " << index << endl;
    mqtt_manager->publish("/er/music/play", "21");

    char index_str[8] = {""};
    itoa(index, index_str, 10);

    static int indexCode;
    indexCode = codes->checkIsCorrect(index_str);

    CodeWithError::CODE_STATE codeState;
    codeState = code16->check(index_str);
    if (codeState == CodeWithError::CODE_STATE::CORRECT)
        indexCode = 15;
    else if (codeState == CodeWithError::CODE_STATE::FAIL)
    {
        *console << "code 16 fail" << endl;
        indexCode = -15;
    }

    codeState = code17->check(index_str);
    if (codeState == CodeWithError::CODE_STATE::CORRECT)
        indexCode = 16;
    else if (codeState == CodeWithError::CODE_STATE::FAIL)
    {
        *console << "code 17 fail" << endl;
        indexCode = -16;
    }
        
    if (indexCode == -1)
    {
        // *console << "Need more numbers" << endl;
        return;
    }
    if (indexCode == -2)
    {
        codes_onFail(indexCode);
        return;
    }
    
    static bool codes_state[codes_ns::CODES_COUNT + 2] = {};

    t_c_codesCheck->launch(DELAY_1_SEC, 1, [](void*){
        *console << "t_c_codesCheck_cb" << endl;
        if (indexCode == -15
        || indexCode == -16)
        {
            codes_onFail(indexCode);
        }
        else 
        if (!codes_state[indexCode])
        {
            codes_state[indexCode] = true;
            code_onCorrect(indexCode);
        }
        else
        {
            codes_onFail(-3);
        }
    });
}

void gerkonsCodes_onActivated(int index)
{
    if (codes_stage != CODES_STAGE_GAME)
        return;

    if (t_c_codesCheck->is_running()
    || t_c_blinkRed->is_running()
    || t_c_blinkGreen->is_running()
    || t_c_delayCodeFail->is_running())
        return;
    
    // *console << "gerkonsCodes_onActivated: " << index << endl;

    ledsNumbers->set(index, WHITE);
    codes_check(index + 1);
}


void t_c_digitsShow_cb(void*)
{
    *console << "t_c_digitsShow_cb" << endl;
    static bool state = false;
    state = !state;
    static size_t count = 0;
    ledsNumbers->clear();       
        
    if (state)
    {
        ledsNumbers->set(codes_ns::DIGITS_SHOW[count], WHITE);
        t_c_digitsShow->launch(DELAY_LEDS_OFF, 2);
    }    
    else
    {
        count++;
        t_c_digitsShow->launch(DELAY_LEDS_ON, 2);
    }

    if (count >= codes_ns::DIGITS_SHOW_COUNT)
    {
        count = 0;
        t_c_digitsShow->launch(DELAY_LEDS_NEXT_SHOW, 2);
    }
}

void codes_init()
{
    for (size_t i = 0; i < codes_ns::LEDS_PINS_COUNT; i++)
        ledsCodes[i] = new SimpleLed(&PCF_3, codes_ns::LEDS_PINS[i], HIGH);

    for (size_t i = 0; i < codes_ns::LEDS_PINS_COUNT; i++)
        ledsCodes[i]->off();
        
    codes = new Codes(codes_ns::CODES_COUNT, codes_ns::CODES);
    code16 = new CodeWithError(codes_ns::CODE_16, codes_ns::CODES_FAIL_COUNT, codes_ns::CODES_16_FAILS);
    code17 = new CodeWithError(codes_ns::CODE_17, codes_ns::CODES_FAIL_COUNT, codes_ns::CODES_17_FAILS);
    
    t_c_codesCheck = new Timer();
    t_c_blinkRed = new Timer();
    t_c_blinkGreen = new Timer();
    t_c_delayCodeFail = new Timer();
    t_c_digitsShow = new Timer(t_c_digitsShow_cb);

    codes_stage = CODES_STAGE_NONE;
    strcpy(props_states[CODES_STATE_POS], MQTT_STRSTATUS_READY);

    ///debug
    // codes_onActivate();
}

void codes_routine()
{
    t_c_codesCheck->routine();
    t_c_blinkRed->routine();
    t_c_blinkGreen->routine();
    t_c_delayCodeFail->routine();
    t_c_digitsShow->routine();

    if (codes_stage != CODES_STAGE_GAME)
        return;
}