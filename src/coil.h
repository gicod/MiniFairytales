#pragma once
#include "common.h"

enum
{
    COIL_STAGE_NONE,
    COIL_STAGE_GAME,
    COIL_STAGE_DONE
} coil_stage;

void coil_onActivate()
{
    console->println(F("coil: onActivated"));
    coil_stage = COIL_STAGE_GAME;
    strcpy(props_states[COIL_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void coil_onFinish()
{
    console->println(F("coil: onFinish"));
    coil_stage = COIL_STAGE_DONE;
    strcpy(props_states[COIL_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void coil_init()
{

    coil_stage = COIL_STAGE_NONE;
    strcpy(props_states[COIL_STATE_POS], MQTT_STRSTATUS_READY);
}

void coil_routine()
{
}