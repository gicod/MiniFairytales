#pragma once
#include "common.h"
#include "hw_devices.h"

enum
{
    RAINBOW_STAGE_NONE,
    RAINBOW_STAGE_GAME,
    RAINBOW_STAGE_DONE
} rainbow_stage;

//r_ -> rainbow
Timer *t_r_blink;
Timer *t_r_delayHint;
Timer *t_r_audioHintOff;
Timer *t_r_audioFinishOff;

void rainbow_onActivate()
{
    console->println(F("rainbow: onActivated"));
    rainbow_stage = RAINBOW_STAGE_GAME;
    strcpy(props_states[RAINBOW_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void rainbow_onFinish()
{
    console->println(F("rainbow: onFinish"));
    mqtt_manager->publish("/er/music/play", rainbow_ns::SOUND_FINISH[complexity]);
    t_r_audioFinishOff->launch(rainbow_ns::DELAY_AUDIO_FINISH_OFF[complexity][current_language], 1, [](void*){
        *console << "t_r_audioFinishOff_cb" << endl;
        mqtt_manager->publish("/er/color_rings/cmd", "activate");
        mqtt_manager->publish("/er/digits/cmd", "activate");
        mqtt_manager->publish("/er/coil/cmd", "activate");
        mqtt_manager->publish("/er/codes/cmd", "activate");
        mqtt_manager->publish("/er/hints/cmd", "activate");
    });
    rainbow_stage = RAINBOW_STAGE_DONE;
    strcpy(props_states[RAINBOW_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void rainbow_onFail()
{
    *console << "rainbow_onFail" << endl;    
    delay(1000);
    mqtt_manager->publish("/er/music/play", rainbow_ns::SOUND_FAIL);
    static bool state;
    state = false;
    t_r_blink->launch(300, 6, [](void*){
        *console << "t_r_blink_cb" << endl;
        state = !state;
        if (state)
        {
            ledsNumbers->set(WHITE);
        }
        else
        {
            ledsNumbers->clear();
        }   
    });

    static size_t count = 0;
    count++;
    if (count == 5)
    {
        t_r_delayHint->launch(rainbow_ns::DELAY_HINT, 1, [](void*){
            *console << "t_r_delayHint_cb" << endl;
            mqtt_manager->publish("/er/music/play", "");
            t_r_audioHintOff->launch(rainbow_ns::DELAY_HINT_OFF[current_language], 1, [](void*){
                *console << "t_r_delayHint_cb" << endl;
            });
        });
    }    
}

void gerkonsRainbow_onActivated(int index) 
{
    if (rainbow_stage != RAINBOW_STAGE_GAME)
        return;
    if (t_r_blink->is_running()
    || t_r_delayHint->is_running()
    || t_r_audioHintOff->is_running())
        return;
    *console << "gerkonsRainbow_onActivated: " << index << endl;
    mqtt_manager->publish("/er/music/play", rainbow_ns::SOUND_GERKON);
    ledsNumbers->set(index, rainbow_ns::COLORS[index]);
    static size_t seq[rainbow_ns::SEQ_COUNT];
    static size_t count = 0;
    seq[count] = index;
    count++;
    if (count == rainbow_ns::SEQ_COUNT)
    {
        for (size_t i = 0; i < rainbow_ns::SEQ_COUNT; i++)
        {
            if (seq[i] != rainbow_ns::SEQ[complexity][i])
            {
                count = 0;
                rainbow_onFail();
                return;
            }            
        }
        delay(1000);
        rainbow_onFinish();
    }
}

void rainbow_init()
{
    t_r_blink = new Timer();
    t_r_delayHint = new Timer();
    t_r_audioHintOff = new Timer();
    t_r_audioFinishOff = new Timer();

    rainbow_stage = RAINBOW_STAGE_NONE;
    strcpy(props_states[RAINBOW_STATE_POS], MQTT_STRSTATUS_READY);
}

void rainbow_routine()
{
    t_r_blink->routine();
    t_r_delayHint->routine();
    t_r_audioHintOff->routine();
    t_r_audioFinishOff->routine();
}