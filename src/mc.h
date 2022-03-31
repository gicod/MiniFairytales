#pragma once
#include <ds_mp3_player.h>
#include <ds_timer.h>
#include "common.h"

using namespace mc_ns;

ArdSensor* btnStart;
ArdSensor* btnReset;

MP3_player<>* back_mc, *action_mc;
Timer *general_info_publisher;

constexpr bool DEBUG_MC = false;

/*
*   mc1 -> action_mc
*   mc2 -> back_mc
*/

constexpr uint16_t LANG_SHIFT = 3U; // sounds, hint1, hint2
enum sound_pos_t {SOUNDS_POS, HINT_1_POS, HINT_2_POS};
typedef bool(*mc_handler_t)(char*, char*);

void mc_onActivate()
{   
    console->println(F("mc_onActivate")); 
    back_mc->play(1,1);
    strcpy(props_states[MC_STATE_POS], MQTT_STRSTATUS_ENABLED); 
}

void mc_onFinish()
{
    console->println(F("mc_onFinish"));
    action_mc->play(2,1);//debug
    strcpy(props_states[MC_STATE_POS], MQTT_STRSTATUS_FINISHED);    
}

void mc_onReset() // simply on_er_reset()
{
    console->println(F("mc_onReset"));
    back_mc->pause();
    action_mc->play(1,2);
    console->telnet_stop();
    wdTimer_reset();
}

static inline uint16_t get_num(char* src)
{
    return static_cast<uint16_t>(atoi(src));
}

static inline uint16_t get_sound_folder(const sound_pos_t &sound_type)
{
    uint16_t shift_by_lang = (current_language_mc - 1);
    shift_by_lang *= LANG_SHIFT;
    return action_mc->get_folder() + shift_by_lang + sound_type;
}

static bool hints_handler(char* topic, char* payload)
{
    bool res = true;
    if (!DEBUG_MC)
        res = false;
    
    if (strcmp(topic, "/er/hint1") == 0) {
        uint16_t soundNum = get_num(payload);
        action_mc->play(get_sound_folder(HINT_1_POS), soundNum);
    } else if (strcmp(topic, "/er/hint2") == 0) {
        uint16_t soundNum = get_num(payload);
        action_mc->play(get_sound_folder(HINT_2_POS), soundNum);
    } else if (strcmp(topic, "/er/autohints/enabled") == 0) {
        action_music_space::AUTOHINTS_ENABLED = static_cast<bool>(get_num(payload));
    } else if (action_music_space::AUTOHINTS_ENABLED) {
        if (strcmp(topic, "/er/autohint1") == 0) {
            action_mc->play(get_sound_folder(HINT_1_POS), get_num(payload));
        } else if (strcmp(topic, "/er/autohint2") == 0) {
            action_mc->play(get_sound_folder(HINT_2_POS), get_num(payload));
        } else {
            res = false;
        }
    } else {
        res = false;
    }

    if (res) {
        console->print(F("\thints_handler: "));
        *console << topic << ": " << payload << endl;
    }

    return res;
}

static bool user_setups_handler(char* topic, char* payload)
{
    bool res = true;
    if (!DEBUG_MC)
        res = false;

    if (strcmp(topic, "/er/mc1/vol/set") == 0) {
        action_mc->set_volume(get_num(payload));
    } else if (strcmp(topic, "/er/mc1/lang/set") == 0) {        
        const uint16_t lang_num = get_num(payload);
        char itoa_buf[8] = {0};
                
        if (lang_num < FIRST_LANGUAGE_IND || lang_num > LAST_LANGUAGE_IND)
            return true;        ///> topic is correspondant but payload is invalid        
        itoa(static_cast<int>(lang_num), itoa_buf, 10);
        // todo: implement retained answer
        current_language_mc = lang_num;
        current_language = lang_num - 1;
    } else if (strcmp(topic, "/er/mc2/vol/set") == 0) {
        back_mc->set_volume(get_num(payload));
    } else if (strcmp(topic, "/er/gamelevel/cmd") == 0) {
            if (strcmp(payload, "basic") == 0)
                complexity = COMPLEXITY::BASIC;
            else if (strcmp(payload, "advanced") == 0)
                complexity = COMPLEXITY::ADVANCED;
    } else {
        res = false;
    }

    if (res) {
        console->print(F("\tuser_setups_handler: "));
        *console << topic << ": " << payload << endl;
    }
    return res;
}

static bool music_handler(char* topic, char* payload)
{   
    bool res = true;
    if (!DEBUG_MC)
        res = false;

    if (strcmp(topic, "/er/musicback/play") == 0) {
        back_mc->play(current_language_mc, get_num(payload));
    } else if(strcmp(topic, "/er/music/stop") == 0) {
        action_mc->pause();
    } else if(strcmp(topic, "/er/mc2/pause") == 0) {
        back_mc->pause();
    } else if(strcmp(topic, "/er/mc2/resume") == 0) {
        back_mc->resume();
    } else {
        res = false;
    }

    if (res) {
        console->print(F("\tmusic_handler: "));
        *console << topic << ": " << payload << endl;
    }
    return res;
}

void special_mc_cb(char* topic, uint8_t* payload, unsigned int len)
{
    constexpr size_t MC_HANDLERS_COUNT = 3U;
    static constexpr mc_handler_t mc_handlers[MC_HANDLERS_COUNT] =
        {hints_handler, user_setups_handler, music_handler };
    auto payload_p = reinterpret_cast<char*>(payload);
    payload[len] = '\0';

    if (DEBUG_MC)
        *console << F("special_mc_cb:\t")
                << F("topic: ") << topic
                << F("\tpayload: ") << payload_p << endl;

    if(strcmp(topic, "/er/music/play") == 0) {
        const auto song_num = get_num(payload_p);

        if (DEBUG_MC)
            *console << F("folder: ") << get_sound_folder(SOUNDS_POS) << endl;

        for (size_t i = 0; i < action_music_space::MAKE_SURE_COUNT; ++i)
            action_mc->play(get_sound_folder(SOUNDS_POS), song_num);   
    } else if(strcmp(topic, "/er/mc1/pause") == 0) {
        action_mc->pause();
    } else if(strcmp(topic, "/er/mc1/resume") == 0) {
        action_mc->resume();
    } else if(strcmp(topic, "/er/music/reset") == 0) {
        mc_onReset();
    } else if(strcmp(topic, "/er/cmd") == 0) {
        if(strcmp(payload_p, "start") == 0) {
            back_mc->play(1);
        } else if(strcmp(payload_p, "reset") == 0) {
            back_mc->pause();      
            action_mc->play(1, 2); //  play "reset all..."  //
            delay(5000);
            mc_onReset();
        } else if(strcmp(payload_p, "finish") == 0) {
            back_mc->pause();      
        }
    }

    for (size_t i = 0; i < MC_HANDLERS_COUNT; ++i)
        if (mc_handlers[i])
            if (mc_handlers[i](topic, payload_p))
                return;
}

constexpr size_t buf_size = 200U;
static char buf[buf_size] = {0};
char* mc_info(const char* strId, MP3_player<HardwareSerial> *mc)
{
    //  {"strId":"mc1", "vol":"50", "lang":"2"}    
    memset(buf, 0, buf_size);
    strcat(buf, "{");

    //  strId  //
    strcat(buf, "\"strId\":\"");
    strcat(buf, strId);
    strcat(buf, "\", ");

    //  vol  //
    strcat(buf, "\"vol\":\"");
    char strVal1[8];
    itoa(mc->get_volume(), strVal1, 10);
    strcat(buf, strVal1);
    strcat(buf, "\", ");

    //  lang  //
    strcat(buf, "\"lang\":\""); // folder_number == lang
    char strVal2[8];
    itoa(mc->get_folder(), strVal2, 10);
    strcat(buf, strVal2);
    strcat(buf, "\"");

    //  end  //
    strcat(buf, "}");

    return buf;
}

char* sound_list_item_info(int id, const char *strName)
{
    memset(buf, 0, buf_size);
    strcat(buf, "{");

    //  strId  //
    strcat(buf, "\"strId\":\"");
    char strVal1[8];
    itoa(id, strVal1, 10);
    strcat(buf, strVal1);
    strcat(buf, "\", ");

    //  strName  //
    strcat(buf, "\"strName\":\"");
    strcat(buf, strName);
    strcat(buf, "\"");

    //  end  //
    strcat(buf, "}");

    return buf;

}

void mc_init()
{
    btnStart = new ArdSensor(mc_ns::BTN_START_PIN, LOW, 50);
        btnStart->setCallbackOnActivated([](){
            // *console << "btnStart_onActivated" << endl;
            mqtt_manager->publish("/er/cmd", "start");
        });
    btnReset = new ArdSensor(mc_ns::BTN_RESET_PIN, LOW, 50);
        btnReset->setCallbackOnActivated([](){
            // *console << "btnReset_onActivated" << endl;
            mqtt_manager->publish("/er/cmd", "reset");
        });

    static constexpr unsigned long mqtt_heart_beat_offset = 1000UL;
    back_mc = new MP3_player<>
        (back_music_space::hwserial, back_music_space::folder, back_music_space::volume);

    action_mc = new MP3_player<>
        (action_music_space::hwserial, action_music_space::folder, action_music_space::volume);
    
    general_info_publisher = new Timer([](void*){
        mqtt_manager->publish("/er/music/info", mc_info("mc2", back_mc));
        mqtt_manager->publish("/er/music/info", mc_info("mc1", action_mc));
    });
    
    strcpy(props_states[MC_STATE_POS], MQTT_STRSTATUS_READY);
    general_info_publisher->launch(mqtt_heart_beat_offset, Timer::TIMER_LOOP);
}

void mc_routine()
{
    btnStart->check();
    btnReset->check();

    static bool is_welcoming_played = false;
    if (!is_welcoming_played && mqtt_manager->is_connected()) {
        action_mc->play(1, 1); // play "welcome on board, capitan..."
        is_welcoming_played = true;
    }

    general_info_publisher->routine();
}