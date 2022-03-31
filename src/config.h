#pragma once
#include <Arduino.h>
#include <PCF8575.h>
#include <NArdSensor.h>
#include <ds_led.h>
#include <ds_maglock.h>
#include <ds_timer.h>
#include <ds_lstrip.h>
#include <ds_cpz_manager.h>

constexpr char CIRCUIT_NAME[] = "MiniFairytales_EK1_Stamp_Merlin_Rainbow_Color_rings_Digits_Coil_Codes_Hints_Mc";
constexpr byte IP_ENDING = 51;
constexpr bool UPLOAD_BOOT_INFO = true;

constexpr uint16_t LANGUAGE_COUNT = 3U;
uint16_t current_language = 0;

constexpr size_t COMPLEXITY_COUNT = 2U;
enum COMPLEXITY {
    BASIC,
    ADVANCED,
} complexity;

constexpr uint8_t pcf8575_addr = 0x23U; // stamp, color_rings
PCF8575 PCF(pcf8575_addr);
constexpr uint8_t pcf8575_addr_2 = 0x22U; // rainbow, codes
PCF8575 PCF_2(pcf8575_addr_2);
constexpr uint8_t pcf8575_addr_3 = 0x25U; // codes
PCF8575 PCF_3(pcf8575_addr_3);
constexpr uint8_t pcf8575_addr_4 = 0x20U; // hints gerkons
PCF8575 PCF_4(pcf8575_addr_4);
constexpr uint8_t pcf8575_addr_5 = 0x21U; // hints leds
PCF8575 PCF_5(pcf8575_addr_5);

constexpr uint32_t DELAY_1_SEC = 1000;
constexpr uint32_t DELAY_2_SEC = 1000;

namespace stamp_ns
{
    constexpr char SOUND_HINT[] = "1";
    constexpr char SOUND_FIRST_GERKON[] = "100";//!
    constexpr char SOUND_FAIL[] = "10";
    constexpr char SOUND_FINISH[] = "11";

    constexpr uint32_t DELAY_ACTIVATE_2 = 3000;
    constexpr uint32_t DELAY_AUDIO_HINT_OFF[LANGUAGE_COUNT] = {
        3000, 3000, 3000
    };
    constexpr uint32_t DELAY_FINISH_2 = 3000;
    constexpr uint32_t DELAY_FINISH_3[LANGUAGE_COUNT] = {
        3000, 3000, 3000
    };

    constexpr uint8_t GERKONS_STAMP_PINS_COUNT = 8;
    constexpr uint8_t GERKONS_STAMP_PINS[GERKONS_STAMP_PINS_COUNT] = {
        3,4,5,6,7,0,1,2//+
    };
    constexpr size_t SEQ_STAMP_COUNT = 4;
    constexpr int SEQ_STAMP[SEQ_STAMP_COUNT] = {4,0,5,2};
    constexpr uint8_t ALGO_ROTATION_STAMP[SEQ_STAMP_COUNT] = {
        1,0,0,1
    };
}

namespace merlin_ns
{
    constexpr char SOUND_FINISH[] = "12";
    constexpr uint32_t DELAY_ACT_RAINBOW[LANGUAGE_COUNT] = {
        3000, 3000, 3000
    };
    constexpr size_t CPZ_SETS_COUNT = 2;
    constexpr size_t CPZ_PINS_COUNT = 1;
    constexpr uint8_t CPZ_PINS[CPZ_PINS_COUNT] = {26};

    CPZ_addr_t SET_1[CPZ_PINS_COUNT] = {
        {0x7C, 0xFA, 0xC4, 0x00, 0x05, 0x00}, // pin 26
    };

    CPZ_addr_t SET_2[CPZ_PINS_COUNT] = {
        {0xEE, 0x70, 0xC5, 0x00, 0x05, 0x00}, // pin 26
    };
}

namespace rainbow_ns
{
    constexpr char SOUND_GERKON[] = "13";
    constexpr char SOUND_FAIL[] = "14";
    constexpr char SOUND_HINT[] = "15";
    const char* SOUND_FINISH[COMPLEXITY_COUNT] = {"16", "17"};

    constexpr uint32_t DELAY_HINT = 4000;
    constexpr uint32_t DELAY_HINT_OFF[LANGUAGE_COUNT] = {
        3000, 3000, 3000
    };
    constexpr uint32_t DELAY_AUDIO_FINISH_OFF[COMPLEXITY_COUNT][LANGUAGE_COUNT] = {
        {3000, 3000, 3000},
        {3000, 3000, 3000},
    };

    constexpr size_t NUMBERS_COUNT = 6;
    Color COLORS[NUMBERS_COUNT] = {
        CYAN,
        ORANGE,
        GREEN,
        YELLOW,
        RED,
        BLUE
    };

    constexpr size_t SEQ_COUNT = 6;
    constexpr size_t SEQ[COMPLEXITY_COUNT][SEQ_COUNT] = {
        {4,1,3,2,0,5},
        {4,1,3,2,0,5},
    };
}

namespace color_rings_ns
{
    constexpr char SOUND_TOGGLE_INDEX[] = "18";
    constexpr char SOUND_SWITCH_COLOR[] = "19";
    constexpr char SOUND_FINISH[] = "20";

    constexpr uint32_t DELAY_BLINK = 500;

    constexpr uint8_t BTN_LEFT_PIN = 8;//+
    constexpr uint8_t BTN_RIGHT_PIN = 9;//+
    constexpr uint8_t LED_BTNS_PIN = 10;//+

    constexpr uint8_t LSTRIPS_PIN = 25;//+
    constexpr uint8_t LSTRIPS_COUNT = 24;
    constexpr EOrder RGB_ORDER = GRB;
    constexpr size_t SEGMENT_COUNT = 24;
    
    constexpr size_t CIRCLE_COUNT = 6;
    constexpr Color COLORS_CIRCLE[CIRCLE_COUNT] = {
        BLUE, PURPLE, YELLOW, GREEN, ORANGE, RED
    };
    
    constexpr Color COLORS_START[SEGMENT_COUNT] = {
        ORANGE, ORANGE, RED,//<-
        GREEN, BLUE, RED, PURPLE, GREEN, YELLOW, BLUE,//->
        YELLOW, BLUE, GREEN,//<-
        RED, GREEN, YELLOW, ORANGE, PURPLE,//->
        PURPLE, BLUE,//<-
        YELLOW, RED, YELLOW,//->
        PURPLE//<-
    };
    constexpr Color COLORS_COMBO[SEGMENT_COUNT] = {
        YELLOW, PURPLE, BLUE,
        BLUE, BLUE, PURPLE, PURPLE, YELLOW, YELLOW, YELLOW,
        YELLOW, PURPLE, BLUE,
        GREEN, GREEN, GREEN, ORANGE, ORANGE,
        ORANGE, GREEN,
        RED, RED, RED,
        RED
    };

    constexpr size_t NEW_INDEX[LSTRIPS_COUNT] = {
        14,18,21,
        22,20,19,17,15,12,13,
        11,16,23,
        5,6,7,8,10,9,4,
        1,2,3,
        0
    };
    
    constexpr uint32_t DELAY_LEDS_ON = 1000;
    constexpr uint32_t DELAY_LEDS_OFF = 2000;
    constexpr uint32_t DELAY_LEDS_NEXT_SHOW = 5000;
    constexpr size_t RINGS_SHOW_COUNT = 6;
    // constexpr size_t RINGS_SHOW[RINGS_SHOW_COUNT] = {1,3,4,5,6,2};
    constexpr size_t RINGS_SHOW[RINGS_SHOW_COUNT] = {0,2,3,4,5,1}; //psw 134562 
}

namespace digits_ns
{
    constexpr uint32_t DELAY_LEDS_ON = 1000;
    constexpr uint32_t DELAY_LEDS_OFF = 2000;
    constexpr uint32_t DELAY_LEDS_NEXT_SHOW = 5000;

    constexpr uint8_t LSTRIPS_PIN = 23;//+
    constexpr uint8_t LSTRIPS_COUNT = 31;
    constexpr EOrder RGB_ORDER = GRB;

    constexpr uint8_t NUMBERS_COUNT = 3;
    constexpr bool NUMBERS_STATE_LEDS[NUMBERS_COUNT][LSTRIPS_COUNT] = {
        {1,1,1,1,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1},//63+
        {1,1,1,1,1,1,1,1,0,0,1,0,1,0,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0},//42+
        {1,1,1,1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,1,1,1,0,1,0,1,1,1,1,1,1,0},//75+
    };
}

namespace coil_ns
{
    constexpr size_t COIN_PIN = 31; //+
}

namespace codes_ns
{
    constexpr char SOUND_GERKON[] = "21";
    constexpr char SOUND_FAIL[] = "22";
    constexpr char SOUND_CORRECT[] = "23";
    constexpr char SOUND_FINISH[] = "24";
    constexpr char SOUND_FAIL_16_PAS[] = "50";
    constexpr char SOUND_FAIL_17_PAS[] = "51";
    
    constexpr uint32_t DELAY_LEDS_ON = 1000;
    constexpr uint32_t DELAY_LEDS_OFF = 2000;
    constexpr uint32_t DELAY_LEDS_NEXT_SHOW = 5000;
    constexpr size_t DIGITS_SHOW_COUNT = 4;
    constexpr size_t DIGITS_SHOW[DIGITS_SHOW_COUNT] = {4,1,5,2};

    constexpr uint8_t LEDS_PINS_COUNT = 15;
    constexpr uint8_t LEDS_PINS[LEDS_PINS_COUNT] = {
        1,0,14,13,12,11,10,9,8,7,6,5,4,3,2,//+
    };

    constexpr size_t CORRECT_CODES_COUNT[COMPLEXITY_COUNT] = {
        12,15
    };

    constexpr size_t CODES_COUNT = 15;
    const String CODES[CODES_COUNT] = {
        "432615",
        "536412",
        "514632",
        "451263",
        "312465",
        "352146",
        "164532",
        "461235",
        "254136",
        "134562",
        "523614",
        "154623",
        "215436",
        "652314",
        "634215", 
    };

    constexpr size_t CODES_FAIL_COUNT = 6;
    const String CODE_16 = "561324";
    const String CODES_16_FAILS[CODES_FAIL_COUNT] = {
        "651324", 
        "563124", 
        "561342", 
        "651342", 
        "653124", 
        "653142",
    };
    const String CODE_17 = "145236";
    const String CODES_17_FAILS[CODES_FAIL_COUNT] = {
        "415236", 
        "142536", 
        "145263", 
        "412536", 
        "415263", 
        "412563",
    };

}

namespace hints_ns
{
    constexpr uint8_t GERKONS_HINTS_PINS_COUNT = 6;
    constexpr uint8_t GERKONS_HINTS_PINS[GERKONS_HINTS_PINS_COUNT] = {
        5,6,4,7,3,2//+
    };
    constexpr uint8_t LEDS_PINS_COUNT = 6;
    constexpr uint8_t LEDS_PINS[LEDS_PINS_COUNT] = {
        7,6,5,4,3,2//+
    };

    constexpr size_t HINTS_COUNT = 17;
    
    constexpr uint32_t DELAY_HINT[LANGUAGE_COUNT][HINTS_COUNT] = {
        {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000},
        {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000},
    };
}

namespace mc_ns
{    
    constexpr uint8_t FIRST_LANGUAGE_IND = 1U;
    constexpr uint8_t LAST_LANGUAGE_IND = 5U;    
    uint16_t current_language_mc = 1U;

    constexpr uint8_t BTN_START_PIN = 36; //+
    constexpr uint8_t BTN_RESET_PIN = 34; //+

    namespace back_music_space {
        HardwareSerial *hwserial = &Serial3;//+
        uint8_t folder = 1U;
        uint8_t volume = 80U;
    }

    namespace action_music_space {        
        HardwareSerial *hwserial = &Serial2;//+
        uint8_t folder = 2U; // the start folder 
        uint8_t volume = 80U;
        bool AUTOHINTS_ENABLED = true;
        constexpr uint8_t MAKE_SURE_COUNT = 1;//! 3U;
    }
}

namespace devices_ns
{
    constexpr uint8_t PCF_2_GERKONS_PINS_COUNT = 6;
    constexpr uint8_t PCF_2_GERKONS_PINS[PCF_2_GERKONS_PINS_COUNT] = {
        0,1,2,3,4,5//+
    };

    constexpr uint8_t LSTRIPS_NUMBERS_PIN = 24;//+
    constexpr uint8_t LSTRIPS_NUMBERS_COUNT = 20;    
    constexpr size_t LSTRIPS_NUMBERS_IN_EACH_SEGM_COUNT = 6;
    constexpr size_t LSTRIPS_NUMBERS_IN_EACH_SEGM[] = {2,4,4,3,4,3};
    constexpr EOrder RGB_ORDER = GRB;
}

const char* topics[] = {
    "/er/gamelevel/cmd",
    "/er/test/cmd",

    "/er/mc1/pause",
    "/er/mc1/resume",
    "/er/mc1/vol/set",
    "/er/mc1/lang/set",
    
    "/er/mc2/pause",
    "/er/mc2/resume",
    "/er/mc2/vol/set",

    // "/er/mc3/play",
    // "/er/mc3/pause",
    // "/er/mc3/resume",
    // "/er/mc3/vol/set",

    "/er/music/play",
    "/er/music/stop",
    // "/er/music/reset",
    
    "/er/musicback/play",

    // "/er/hint1",
    // "/er/hint2",
    
    // "/er/autohint1",
    // "/er/autohint2",
    // "/er/autohints/enabled",
};
constexpr size_t topics_count = sizeof(topics)/sizeof(topics[0]);