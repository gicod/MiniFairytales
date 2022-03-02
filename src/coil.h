#pragma once
#include "common.h"

enum
{
    COIL_STAGE_NONE,
    COIL_STAGE_GAME,
    COIL_STAGE_DONE
} coil_stage;

SimpleLed *coin;

void coil_onActivate()
{
    console->println(F("coil: onActivated"));
    coin->on();
    coil_stage = COIL_STAGE_GAME;
    strcpy(props_states[COIL_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void coil_onFinish()
{
    console->println(F("coil: onFinish"));
    coin->off();
    coil_stage = COIL_STAGE_DONE;
    strcpy(props_states[COIL_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void coil_init()
{
    coin = new SimpleLed(coil_ns::COIN_PIN);
    coil_stage = COIL_STAGE_NONE;
    strcpy(props_states[COIL_STATE_POS], MQTT_STRSTATUS_READY);
}

void coil_routine()
{
}