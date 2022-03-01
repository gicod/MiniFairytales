#pragma once
#include "common.h"

using namespace stamp_ns;

enum
{
    STAMP_STAGE_NONE,
    STAMP_STAGE_FIRST_GERKON,
    STAMP_STAGE_GAME,
    STAMP_STAGE_DONE
} stamp_stage;

// s_ -> stamp
enum STAMP_ROTATION
{
    COUNTER_CLOCKWISE,
    CLOCKWISE, 
    ERROR = 666,
} s_rotation;

ArdSensor *gerkonsStamp[stamp_ns::GERKONS_STAMP_PINS_COUNT];

Timer *t_s_delayActivate2;
Timer *t_s_audioHintOff;;
Timer *t_s_delayFinish2;
Timer *t_s_delayFinish3;

uint8_t s_count = 0;
int s_last_gerkon = -1;

void stamp_onActivate()
{
    console->println(F("stamp: onActivated"));
    t_s_delayActivate2->launch(stamp_ns::DELAY_ACTIVATE_2, 1, [](void*){
        *console << "t_s_delayActivate2_cb" << endl;
        mqtt_manager->publish("/er/music/play", stamp_ns::SOUND_HINT);
        t_s_audioHintOff->launch(stamp_ns::DELAY_AUDIO_HINT_OFF[current_language], 1, [](void*){
            *console << "t_s_audioHintOff_cb" << endl;
            stamp_stage = STAMP_STAGE_FIRST_GERKON;
        });
    });
    strcpy(props_states[STAMP_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void stamp_onFinish()
{
    console->println(F("stamp: onFinish"));
    mqtt_manager->publish("/er/music/play", stamp_ns::SOUND_FINISH);
    t_s_delayFinish3->launch(stamp_ns::DELAY_FINISH_3[current_language], 1, [](void*){
        *console << "t_s_delayFinish3_cb" << endl;
        mqtt_manager->publish("/er/merlin/cmd", "activate");
    });
    stamp_stage = STAMP_STAGE_DONE;
    strcpy(props_states[STAMP_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

bool rotation_isCorrect(int index)
{
    if ((index == (s_last_gerkon - 1)) 
    || (index == (s_last_gerkon + GERKONS_STAMP_PINS_COUNT - 1)))
    {
        s_rotation = COUNTER_CLOCKWISE;
    }
    else 
    if ((index == (s_last_gerkon + 1)) 
    || (index == (s_last_gerkon - 7))) // 7 = GERKONS_STAMP_PINS_COUNT - 1
    {
        s_rotation = CLOCKWISE;
    }
    else
    {
        s_rotation = ERROR;
    }
    s_last_gerkon = index;
    *console << "  rotation -> " << s_rotation << endl;
    if (s_rotation == ALGO_ROTATION_STAMP[s_count])
        return true;
    return false;    
}

void stamp_onCorrect(int index)
{
    *console << "stamp_onCorrect" << endl;
    s_count++;
    if (s_count == SEQ_STAMP_COUNT)
    {
        t_s_delayFinish2->launch(stamp_ns::DELAY_FINISH_2, 1, [](void*){
            *console << "t_s_delayFinish2_cb" << endl;
            stamp_onFinish();
        });
    }
}

void stamp_onFail()
{
    *console << "stamp_onFail" << endl;
    s_count = 0;
    mqtt_manager->publish("/er/music/play", stamp_ns::SOUND_FAIL);
}

void stamp_init()
{
    for (size_t i = 0; i < stamp_ns::GERKONS_STAMP_PINS_COUNT; ++i)
    {
        gerkonsStamp[i] = new ArdSensor(&PCF, stamp_ns::GERKONS_STAMP_PINS[i], LOW, 1);
        gerkonsStamp[i]->setCallbackOnActivatedTagged([](int i){ 
            if (stamp_stage == STAMP_STAGE_FIRST_GERKON)
            {
                s_last_gerkon = i;
                stamp_stage = STAMP_STAGE_GAME;
                *console << "STAMP_STAGE_GAME" << endl;
                mqtt_manager->publish("/er/music/play", stamp_ns::SOUND_FIRST_GERKON);
                return;
            }

            if (i == s_last_gerkon)
                return;
            *console << "gerkonsStamp_onActivated: " << i;// << endl; 

            ///debug
            // char tmp[8] ={""};
            // itoa((i + 1), tmp, 10);
            // mqtt_manager->publish("/er/music/play", tmp);
            ///debug end

            if (rotation_isCorrect(i))
            {        
                char tmp[8] ={""};
                itoa((i + 1), tmp, 10);
                mqtt_manager->publish("/er/music/play", tmp);
                if (i == SEQ_STAMP[s_count])
                {
                    stamp_onCorrect(i);                        
                }
            }
            else
            {
                stamp_onFail();
            }
            
            s_last_gerkon = i;
        },i);
    }

    t_s_delayActivate2 = new Timer();
    t_s_audioHintOff = new Timer();
    t_s_delayFinish2 = new Timer();
    t_s_delayFinish3 = new Timer();

    stamp_stage = STAMP_STAGE_NONE;
    strcpy(props_states[STAMP_STATE_POS], MQTT_STRSTATUS_READY);
}

void stamp_routine()
{
    t_s_delayActivate2->routine();
    t_s_audioHintOff->routine();
    t_s_delayFinish2->routine();
    t_s_delayFinish3->routine();

    if (t_s_delayFinish2->is_running())
        return;
    
    if (stamp_stage == STAMP_STAGE_FIRST_GERKON
    || stamp_stage == STAMP_STAGE_GAME)
    {
        for (auto &gerkons : gerkonsStamp)
            gerkons->check();
    }
}