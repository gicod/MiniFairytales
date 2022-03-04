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
Timer *t_81;
Timer *t_82;
Timer *t_83;
Timer *t_84;
Timer *t_85;
Timer *t_86;

size_t hints_count = 0;
Hint* hints;

void hint_newHint()
{
    *console << "hint_newHint" << endl;
    hints->newHint();
    for (size_t i = 0; i < hints_ns::LEDS_PINS_COUNT; i++)
        if (hints->getIndexLed(i) != -1)
            ledsHints[i]->on();  
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
    hints_stage = HINTS_STAGE_DONE;
    strcpy(props_states[HINTS_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void hints_mc_cb(char* topic, uint8_t* payload, unsigned int len)
{
    // char* payload_p = reinterpret_cast<char*>(payload);
    // payload[len] = '\0';

    // if(strcmp(topic, "/er/hints/cmd") == 0) {
    //     if(strcmp(topic, "stop_hint") == 0) {            
    //         // color_rings_stage = COLOR_RINGS_STAGE_DONE;
    //         // ledsRings->clear();
    //         // t_cr_delay2sec->pause();
    //         // t_cr_showRings->pause();
    //         // //!
    //     }
    // }
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
            *console << "gerkonsHints_onActivated: " << i << endl;
            t_h_showHint->launch(hints_ns::DELAY_HINT[current_language][(size_t)tmp], 1, [](void*){
                *console << "t_h_showHint_cb" << endl;                
            });

            char tmp_c[8] ={""};
            itoa((tmp + 25), tmp_c, 10);
            mqtt_manager->publish("/er/music/play", tmp_c);
        },i);
        ledsHints[i] = new SimpleLed(&PCF_5, hints_ns::LEDS_PINS[i], HIGH);
    }
    for (size_t i = 0; i < hints_ns::GERKONS_HINTS_PINS_COUNT; ++i)
        ledsHints[i]->off();
    
    hints = new Hint(hints_ns::HINTS_COUNT);

    t_h_showHint = new Timer();
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
    t_h_showHint->routine();
    t_81->routine();
    t_82->routine();
    t_83->routine();
    t_84->routine();
    t_85->routine();
    t_86->routine();

    if (hints_stage != HINTS_STAGE_GAME)
        return;

    if (t_h_showHint->is_running())
        return;

    for (size_t i = 0; i < hints_ns::GERKONS_HINTS_PINS_COUNT; ++i)
        gerkonsHints[i]->check();
}