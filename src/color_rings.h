#pragma once
#include "common.h"

enum
{
    COLOR_RINGS_STAGE_NONE,
    COLOR_RINGS_STAGE_GAME,
    COLOR_RINGS_STAGE_DONE
} color_rings_stage;

ArdSensor *btnLeft, *btnRight;
SimpleLed *ledsBtns;
Lstrip *ledsRings;

//cr_ ->color_rings
Timer *t_cr_blink;
Timer *t_cr_delayFinish;
Timer *t_cr_delay2sec;
Timer *t_cr_showRings;

Color colors_current[color_rings_ns::SEGMENT_COUNT];
uint8_t circle_index = 0;
bool led_state = true;

constexpr size_t CIRCLE_CENTERS[color_rings_ns::CIRCLE_COUNT] = {4,6,8,14,16,21};

void blink_newIndex()
{
    for(auto i : CIRCLE_CENTERS)
        ledsRings->set(color_rings_ns::NEW_INDEX[i], color_rings_ns::COLORS_START[i]);

    led_state = true;
    t_cr_blink->launch(color_rings_ns::DELAY_BLINK, Timer::TIMER_LOOP, [](void*){
        // *console << "t_blink_cb" << endl;
        led_state = !led_state;
        if(led_state)
            ledsRings->set(color_rings_ns::NEW_INDEX[CIRCLE_CENTERS[circle_index]], color_rings_ns::COLORS_CIRCLE[circle_index]);
        else
            ledsRings->set(color_rings_ns::NEW_INDEX[CIRCLE_CENTERS[circle_index]], NONE_COLOR);        
    });
}

void color_rings_onActivate()
{
    console->println(F("color_rings: onActivated"));
    ledsBtns->on();
    for (size_t i = 0; i < color_rings_ns::SEGMENT_COUNT; ++i)
    {
        colors_current[i] = color_rings_ns::COLORS_START[i];
        ledsRings->set(color_rings_ns::NEW_INDEX[i], colors_current[i]);
    }
    blink_newIndex();
    color_rings_stage = COLOR_RINGS_STAGE_GAME;
    strcpy(props_states[COLOR_RINGS_STATE_POS], MQTT_STRSTATUS_ENABLED);
}

void color_rings_onFinish()
{
    console->println(F("color_rings: onFinish"));
    t_cr_blink->pause();
    mqtt_manager->publish("/er/music/play", color_rings_ns::SOUND_FINISH);
    // for (size_t i = 0; i < SEGMENT_COUNT; ++i)
    // {
    //     colors_current[i] = COLORS_COMBO[i];
    //     ledsRings->set(NEW_INDEX[i], colors_current[i]);
    // }
    ledsRings->clear();
    t_cr_delay2sec->launch(DELAY_2_SEC, 1, [](void*){
        *console << "t_cr_delay2sec_cb" << endl;
        t_cr_showRings->launch(1,1);
    });
    
    color_rings_stage = COLOR_RINGS_STAGE_DONE;
    strcpy(props_states[COLOR_RINGS_STATE_POS], MQTT_STRSTATUS_FINISHED);
}

void check_colorCombo()
{
    uint8_t counter = 0;
    *console << F("color correct:");
    for (size_t i = 0; i < color_rings_ns::SEGMENT_COUNT; ++i)
    {
        if (colors_current[i] == color_rings_ns::COLORS_COMBO[i])
        {
            counter++;
            *console << " +";
        }
        else
            *console << " -";
        // *console << colors_current[i] << "/" << COLORS_COMBO[i];
    }
    *console << "" << endl;
    if (counter == color_rings_ns::SEGMENT_COUNT)
    {
        t_cr_delayFinish->launch(DELAY_1_SEC, 1, [](void*){
            *console << "t_cr_delayFinish_cb" << endl;
            color_rings_onFinish();
        });
    }
}

void switch_colors()
{
    *console << F("switch_colors: ") << circle_index << endl;
    Color color_temp;
    switch (circle_index)
    {
    case 0:
        color_temp = colors_current[2];
        colors_current[2] = colors_current[3];
        colors_current[3] = colors_current[12];
        colors_current[12] = colors_current[5];
        colors_current[5] = color_temp;
        break;
    case 1:
        color_temp = colors_current[1];
        colors_current[1] = colors_current[5];
        colors_current[5] = colors_current[11];
        colors_current[11] = colors_current[7];
        colors_current[7] = color_temp;
        break;
    case 2:
        color_temp = colors_current[0];
        colors_current[0] = colors_current[7];
        colors_current[7] = colors_current[10];
        colors_current[10] = colors_current[9];
        colors_current[9] = color_temp;
        break;
    case 3:
        color_temp = colors_current[12];
        colors_current[12] = colors_current[13];
        colors_current[13] = colors_current[19];
        colors_current[19] = colors_current[15];
        colors_current[15] = color_temp;
        break;
    case 4:
        color_temp = colors_current[11];
        colors_current[11] = colors_current[15];
        colors_current[15] = colors_current[18];
        colors_current[18] = colors_current[17];
        colors_current[17] = color_temp;
        break;
    case 5:
        color_temp = colors_current[19];
        colors_current[19] = colors_current[20];
        colors_current[20] = colors_current[23];
        colors_current[23] = colors_current[22];
        colors_current[22] = color_temp;
        break;
    }
    for (size_t i = 0; i < color_rings_ns::SEGMENT_COUNT; ++i)
        ledsRings->set(color_rings_ns::NEW_INDEX[i], colors_current[i]);
    check_colorCombo();
}

void t_cr_showRings_cb(void*)
{
    // *console << "t_cr_showRings_cb" << endl;
    static bool state = false;
    state = !state;
    static size_t count = 0;
    ledsRings->clear();       
        
    if (state)
    {
        for (size_t i = 0; i < color_rings_ns::RINGS_SHOW[count]; i++)
            ledsRings->set(color_rings_ns::NEW_INDEX[CIRCLE_CENTERS[i]], GREEN);
        t_cr_showRings->launch(color_rings_ns::DELAY_LEDS_OFF, 2);
    }    
    else
    {
        count++;
        t_cr_showRings->launch(color_rings_ns::DELAY_LEDS_ON, 2);
    }

    if (count >= color_rings_ns::RINGS_SHOW_COUNT)
    {
        count = 0;
        t_cr_showRings->launch(color_rings_ns::DELAY_LEDS_NEXT_SHOW, 2);
    }
}


void color_rings_init()
{
    btnLeft = new ArdSensor(&PCF, color_rings_ns::BTN_LEFT_PIN, LOW, 50);
        btnLeft->setCallbackOnActivated([](){
            // *console << "btnLeft_onActivated" << endl;
            mqtt_manager->publish("/er/music/play", color_rings_ns::SOUND_SWITCH_COLOR);
            switch_colors();
        });
    btnRight = new ArdSensor(&PCF, color_rings_ns::BTN_RIGHT_PIN, LOW, 50);
        btnRight->setCallbackOnActivated([](){
            // *console << "btnRight_onActivated" << endl;
            mqtt_manager->publish("/er/music/play", color_rings_ns::SOUND_TOGGLE_INDEX);
            circle_index++;
            if(circle_index >= color_rings_ns::CIRCLE_COUNT)
                circle_index = 0;
            *console << "circle_index: " << circle_index << endl;  
            blink_newIndex();
        });
    ledsBtns = new SimpleLed(&PCF, color_rings_ns::LED_BTNS_PIN, HIGH);
    ledsRings = new Lstrip(color_rings_ns::LSTRIPS_PIN, color_rings_ns::LSTRIPS_COUNT, color_rings_ns::RGB_ORDER);
    ledsRings->set_segmentation(color_rings_ns::LSTRIPS_COUNT);
    
    t_cr_blink = new Timer();   
    t_cr_delayFinish = new Timer();
    t_cr_delay2sec = new Timer();
    t_cr_showRings = new Timer(t_cr_showRings_cb); 

    color_rings_stage = COLOR_RINGS_STAGE_NONE;
    strcpy(props_states[COLOR_RINGS_STATE_POS], MQTT_STRSTATUS_READY);

    ///debug
    // color_rings_onActivate();
}

void color_rings_routine()
{    
    t_cr_blink->routine();
    t_cr_delayFinish->routine();
    t_cr_delay2sec->routine();
    t_cr_showRings->routine();

    if (color_rings_stage != COLOR_RINGS_STAGE_GAME)
        return;

    if (t_cr_delayFinish->is_running())
        return;
        
    btnLeft->check();
    btnRight->check();
}