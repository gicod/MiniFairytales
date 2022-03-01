#pragma once
#include "common.h"

using namespace merlin_ns;

enum
{
    MERLIN_STAGE_NONE,
    MERLIN_STAGE_GAME,
    MERLIN_STAGE_DONE
} merlin_stage;

CPZ_manager<CPZ_PINS_COUNT, CPZ_SETS_COUNT> *cpz;
CPZ_addrs_t SETS[CPZ_SETS_COUNT] = {SET_1, SET_2};

Timer *t_actRainbow;

void merlin_onActivate()
{
    console->println(F("merlin: onActivated"));
    merlin_stage = MERLIN_STAGE_GAME;
    strcpy(props_states[MERLIN_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void merlin_onFinish()
{
    console->println(F("merlin: onFinish"));
    mqtt_manager->publish("/er/music/play", merlin_ns::SOUND_FINISH);
    t_actRainbow->launch(DELAY_ACT_RAINBOW[current_language], 1, [](void*){
        *console << "t_actRainbow_cb" << endl;
        mqtt_manager->publish("/er/rainbow/cmd", "activate");
    });
    merlin_stage = MERLIN_STAGE_DONE;
    strcpy(props_states[MERLIN_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void onFound(uint8_t index){
    *console << "onFound: " << index << endl;
};

void onCorrect(uint8_t index){
    *console << "onCorrect: " << index << endl;
};

void onRemoved(uint8_t index){
    *console << "onRemoved: " << index << endl;
};

void merlin_init()
{
    cpz = new CPZ_manager<CPZ_PINS_COUNT, CPZ_SETS_COUNT>(CPZ_PINS, SETS, onFound, onCorrect, onRemoved);
        
    t_actRainbow = new Timer();

    merlin_stage = MERLIN_STAGE_NONE;
    strcpy(props_states[MERLIN_STATE_POS], MQTT_STRSTATUS_READY);
}

void merlin_routine()
{
    t_actRainbow->routine();

    if (merlin_stage != MERLIN_STAGE_GAME)
        return;

    if (cpz->isCorrect())
    {
        merlin_onFinish();
    }
}