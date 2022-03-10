#pragma once
#include "config.h"
#include <ds_bootloader.h>
#include <ds_wdt.h>
#include <ds_mqtt_manager.h>

enum {STAMP_STATE_POS, MERLIN_STATE_POS, RAINBOW_STATE_POS, COLOR_RINGS_STATE_POS, DIGITS_STATE_POS, COIL_STATE_POS, CODES_STATE_POS, HINTS_STATE_POS, MC_STATE_POS};

constexpr size_t PROPS_NUM = 9U;
constexpr int props_num_in_ERP[PROPS_NUM] = {10, 20, 30, 40, 50, 60, 70, 80, 90};

constexpr char PROP_1_STRID[]  = "stamp";
constexpr char PROP_2_STRID[]  = "merlin";
constexpr char PROP_3_STRID[]  = "rainbow";
constexpr char PROP_4_STRID[]  = "color_rings";
constexpr char PROP_5_STRID[]  = "digits";
constexpr char PROP_6_STRID[]  = "coil";
constexpr char PROP_7_STRID[]  = "codes";
constexpr char PROP_8_STRID[]  = "hints";
constexpr char PROP_9_STRID[]  = "mc";
const char *propsNames[PROPS_NUM] = {PROP_1_STRID, PROP_2_STRID, PROP_3_STRID, PROP_4_STRID, PROP_5_STRID, PROP_6_STRID, PROP_7_STRID, PROP_8_STRID, PROP_9_STRID};

void stamp_onActivate();
void stamp_onFinish();
prop_CBs_t  stamp_cbs = {stamp_onActivate, stamp_onFinish, ds_MQTT::reset};

void merlin_onActivate();
void merlin_onFinish();
prop_CBs_t  merlin_cbs = {merlin_onActivate, merlin_onFinish, ds_MQTT::reset};

void rainbow_onActivate();
void rainbow_onFinish();
prop_CBs_t  rainbow_cbs = {rainbow_onActivate, rainbow_onFinish, ds_MQTT::reset};

void color_rings_onActivate();
void color_rings_onFinish();
prop_CBs_t  color_rings_cbs = {color_rings_onActivate, color_rings_onFinish, ds_MQTT::reset};

void digits_onActivate();
void digits_onFinish();
prop_CBs_t  digits_cbs = {digits_onActivate, digits_onFinish, ds_MQTT::reset};

void coil_onActivate();
void coil_onFinish();
prop_CBs_t  coil_cbs = {coil_onActivate, coil_onFinish, ds_MQTT::reset};

void codes_onActivate();
void codes_onFinish();
prop_CBs_t  codes_cbs = {codes_onActivate, codes_onFinish, ds_MQTT::reset};

void hints_onActivate();
void hints_onFinish();
prop_CBs_t  hints_cbs = {hints_onActivate, hints_onFinish, ds_MQTT::reset};

void mc_onActivate();
void mc_onFinish();
void mc_onReset();
prop_CBs_t  mc_cbs = {mc_onActivate, mc_onFinish, mc_onReset};

props_CBs_t props_cbs[PROPS_NUM] = {&stamp_cbs, &merlin_cbs, &rainbow_cbs, &color_rings_cbs, &digits_cbs, &coil_cbs, &codes_cbs, &hints_cbs, &mc_cbs, };

prop_state_t stamp_state = {0};
prop_state_t merlin_state = {0};
prop_state_t rainbow_state = {0};
prop_state_t color_rings_state = {0};
prop_state_t digits_state = {0};
prop_state_t coil_state = {0};
prop_state_t codes_state = {0};
prop_state_t hints_state = {0};
prop_state_t mc_state = {0};
props_states_t props_states[PROPS_NUM] = {stamp_state, merlin_state, rainbow_state, color_rings_state, digits_state, coil_state, codes_state, hints_state, mc_state, };


Console *console;

void on_er_start()
{
  console->println(F("ER: Start"));
  stamp_onActivate();
}

void on_er_reset()
{
  console->println(F("ER: Reset"));
  mc_onReset();
  console->telnet_stop();
  wdTimer_reset();
}

void special_mc_cb(char* topic, uint8_t* payload, unsigned int len);
void colorRings_scb(char* topic, uint8_t* payload, unsigned int len);
void hints_scb(char* topic, uint8_t* payload, unsigned int len);
void special_cbs(char* topic, uint8_t* payload, unsigned int len)
{
  special_mc_cb(topic, payload, len);
  colorRings_scb(topic, payload, len);
  hints_scb(topic, payload, len);
}

typedef  MQTT_manager<
  PROPS_NUM, CIRCUIT_NAME,
  propsNames, props_num_in_ERP,
  on_er_start, on_er_reset, props_cbs,
  special_cbs, topics, topics_count
> mqtt_manager_t;

mqtt_manager_t *mqtt_manager;

void common_routine_init()
{
  if (UPLOAD_BOOT_INFO)
    bootloader_init(IP_ENDING);

  console = new Console(CIRCUIT_NAME);
  mqtt_manager = new mqtt_manager_t(console, IP_ENDING);
  Serial.begin(115200);
  resetServer_init();  
}

void common_routine()
{
  mqtt_manager->routine(props_states);
  console->check();
  wdTimer_check();
  resetServer_check(console);
}
