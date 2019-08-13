#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_partition.h"
#include "driver/i2s.h"
#include "esp_spiffs.h"
#include "nvs_flash.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "esp_ota_ops.h"

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;

#include "../components/lynx/handy.h"
#include "../components/lynx/myadd.h"

#include "../components/odroid/odroid_settings.h"
#include "../components/odroid/odroid_audio.h"
#include "../components/odroid/odroid_input.h"
#include "../components/odroid/odroid_system.h"
#include "../components/odroid/odroid_display.h"
#include "../components/odroid/odroid_sdcard.h"
#include "../components/odroid/odroid_ui.h"
#include "../components/odroid/odroid_ui_choosefile.h"

#include <dirent.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOINLINE  __attribute__ ((noinline))

#define MY_LYNX_INCLUDE_SAVE_TO_SD

#define PALETTE_SIZE 256

const char* SD_BASE_PATH = "/sd";

//#define AUDIO_SAMPLE_RATE (32000)
#define AUDIO_SAMPLE_RATE (22050)

#define AUDIO_BUFFER_SIZE 2756

uint32_t *lynx_mColourMap;
//uint16 palette[PALETTE_SIZE];
uint16 *palette = NULL;
uint16_t* framebuffer[2]; // uint8_t*
int currentFramebuffer = 0;

#ifdef MY_AUDIO_MODE_V1
struct audio_meta {
    //uint8_t *buffer;
    short *buffer;
    int length;
};

extern short *gAudioBuffer;
extern short *gAudioBufferPointer2;

struct audio_meta audio_update1 = {0,};
struct audio_meta audio_update2 = {0,};
struct audio_meta *audio_update = &audio_update1;
QueueHandle_t audioQueue;
TaskHandle_t audioTaskHandle;
#endif

spi_flash_mmap_handle_t hrom;

QueueHandle_t vidQueue;
TaskHandle_t videoTaskHandle;

odroid_volume_level Volume;
odroid_battery_state battery;

bool scaling_enabled = true;
bool config_ui_stats = false;
int8_t filtering = 0;
bool previous_scaling_enabled = true;
uint8_t previous_rotate = 0;
uint8_t previous_filtering = 0;

volatile bool videoTaskIsRunning = false;

const char *odroidgo_rom_path = "/sd/roms/lynx";
retro_pixel_format odroidgo_fmt = RETRO_PIXEL_FORMAT_RGB565;
retro_game_info odroid_game;
retro_system_info retro_info;
int16_t retrolib_input_state_t = 0;

uint startTime;
uint stopTime;
uint totalElapsedTime;
int frame;
volatile float fps_ui = 0;
uint8_t rotate = 0;

typedef void (ODROID_UI_CALLCONV *odroid_display_func_def)(uint8_t* buffer, uint32_t* myPalette);

bool display_func_change = false;
odroid_display_func_def odroid_display_func;

// functions
extern void SaveState();
extern void LoadState();


void update_display_func() {
  previous_scaling_enabled = scaling_enabled;
  previous_filtering = filtering;
  previous_rotate = rotate;
  
  if (!scaling_enabled) {
     odroid_display_func = ili9341_write_frame_lynx_v2_original;
  } else {
    switch (filtering) {
    case 0:
        odroid_display_func = ili9341_write_frame_lynx_v2_mode0;
    break;
    case 1:
        odroid_display_func = ili9341_write_frame_lynx_v2_mode1;
    break;
    case 2:
        odroid_display_func = ili9341_write_frame_lynx_v2_mode2;
    break;
    case 3:
        odroid_display_func = ili9341_write_frame_lynx_v2_mode3;
    break;
    }
  }
  display_func_change = false;
}

#define TASK_BREAK (void*)1

#ifdef MY_VIDEO_MODE_V1

#define VID_TASK(func) \
    uint8_t* param; \
    videoTaskIsRunning = true; \
    printf("%s: STARTED\n", __func__); \
     \
    while(1) \
    { \
        xQueuePeek(vidQueue, &param, portMAX_DELAY); \
 \
        if (param == TASK_BREAK) \
            break; \
 \
        odroid_display_lock(); \
        func(param, lynx_mColourMap); \
        odroid_display_unlock(); \
        /* odroid_input_battery_level_read(&battery);*/ \
        xQueueReceive(vidQueue, &param, portMAX_DELAY); \
    } \
    xQueueReceive(vidQueue, &param, portMAX_DELAY); \
    /*odroid_display_lock();*/ \
    /*odroid_display_show_hourglass();*/ \
    /*odroid_display_unlock();*/ \
    videoTaskIsRunning = false; \
    printf("%s: FINISHED\n", __func__); \
    vTaskDelete(NULL); \
    while (1) {}


void videoTask_mode0(void *arg) { VID_TASK(ili9341_write_frame_lynx_v2_mode0) }
void videoTask_mode1(void *arg) { VID_TASK(ili9341_write_frame_lynx_v2_mode1) }
void videoTask_mode2(void *arg) { VID_TASK(ili9341_write_frame_lynx_v2_mode2) }
void videoTask_mode3(void *arg) { VID_TASK(ili9341_write_frame_lynx_v2_mode3) }
void videoTask_mode_original(void *arg) { VID_TASK(ili9341_write_frame_lynx_v2_original) }
void videoTask_mode_original_rotate_R(void *arg) { VID_TASK(ili9341_write_frame_lynx_v2_original_rotate_R) }
void videoTask_mode_original_rotate_L(void *arg) { VID_TASK(ili9341_write_frame_lynx_v2_original_rotate_L) }
void videoTask_mode_original_mode0_rotate_R(void *arg) { VID_TASK(ili9341_write_frame_lynx_v2_mode0_rotate_R) }
void videoTask_mode_original_mode0_rotate_L(void *arg) { VID_TASK(ili9341_write_frame_lynx_v2_mode0_rotate_L) }

#endif

NOINLINE void update_display_task()
{
    printf("%s: Step #001\n", __func__);
    if (videoTaskIsRunning)
    {
        printf("VIDEO: Task: Stop\n");
        uint16_t* param = TASK_BREAK;
        xQueueSend(vidQueue, &param, portMAX_DELAY);
        while (videoTaskIsRunning) { vTaskDelay(1); }
        printf("VIDEO: Task: Stop done\n");
        
        printf("VIDEO: Clear display\n");
        //odroid_display_lock();
        ili9341_write_frame_lynx(NULL, NULL, false);
        //odroid_display_unlock();
    }
    printf("%s: Step #002\n", __func__);
    TaskFunction_t taskFunc = &videoTask_mode0;
    
      previous_scaling_enabled = scaling_enabled;
      previous_filtering = filtering;
      previous_rotate = rotate;
      
      my_setbutton_mapping(rotate);
      
      if (!scaling_enabled) {
         switch (rotate) {
         case 0: taskFunc = &videoTask_mode_original; break;
         case 1: taskFunc = &videoTask_mode_original_rotate_R; break;
         case 2: taskFunc = &videoTask_mode_original_rotate_L; break;
         }
      } else {
        if (rotate) {
              switch (rotate) {
                case 0: taskFunc = &videoTask_mode_original; break;
                case 1: taskFunc = &videoTask_mode_original_mode0_rotate_R; break;
                case 2: taskFunc = &videoTask_mode_original_mode0_rotate_L; break;
             }
        } else {
          // filtering;
          switch (filtering) {
          case 0:
            taskFunc = &videoTask_mode0;
            break;
          case 1:
            taskFunc = &videoTask_mode1;
            break;
          case 2:
            taskFunc = &videoTask_mode2;
            break;
          case 3:
            taskFunc = &videoTask_mode3;
            break;
          default:
            taskFunc = &videoTask_mode0;
            break;
          }
        }
      }
    printf("%s: Step #003\n", __func__);
    
    printf("VIDEO: Task: Start\n");
    xTaskCreatePinnedToCore(taskFunc, "videoTask", 1024 * 4, NULL, 5, &videoTaskHandle, 1);
    while (!videoTaskIsRunning) { vTaskDelay(1); }
    printf("VIDEO: Task: Start done\n");
}

#ifdef MY_AUDIO_MODE_V1
volatile bool AudioTaskIsRunning = false;
void audioTask(void* arg)
{
  // sound
  struct audio_meta* param;
  
  AudioTaskIsRunning = true;
  while(1)
  {
    xQueuePeek(audioQueue, &param, portMAX_DELAY);

    if (param == NULL)
    {
        break;
    }
    else
    {
       odroid_audio_submit((short*)param->buffer, param->length - 1);
    }
    xQueueReceive(audioQueue, &param, portMAX_DELAY);
  }
  printf("audioTask: exiting.\n");
  odroid_audio_terminate();
  AudioTaskIsRunning = false;
  vTaskDelete(NULL);

  while (1) {}
}
#endif

//Read an unaligned byte.
char unalChar(const unsigned char *adr) {
	//See if the byte is in memory that can be read unaligned anyway.
	if (!(((int)adr)&0x40000000)) return *adr;
	//Nope: grab a word and distill the byte.
	int *p=(int *)((int)adr&0xfffffffc);
	int v=*p;
	int w=((int)adr&3);
	if (w==0) return ((v>>0)&0xff);
	if (w==1) return ((v>>8)&0xff);
	if (w==2) return ((v>>16)&0xff);
	if (w==3) return ((v>>24)&0xff);

    abort();
    return 0;
}

NOINLINE void DoMenuHome(bool save)
{
    uint16_t* param = TASK_BREAK;
    void *exitAudioTask = NULL;

    // Clear audio to prevent studdering
    printf("PowerDown: stopping audio.\n");
#ifdef MY_AUDIO_MODE_V1
    xQueueSend(audioQueue, &exitAudioTask, portMAX_DELAY);
    while (AudioTaskIsRunning) {}
#else
    odroid_audio_terminate();
#endif

    // Stop tasks
    printf("PowerDown: stopping tasks.\n");

    xQueueSend(vidQueue, &param, portMAX_DELAY);
    while (videoTaskIsRunning) { vTaskDelay(1); }

    odroid_settings_ForceInternalGameSelect_set(1);
    DoReboot(save);
}

extern uint32    *gAudioEnabledPointer;

void menu_lynx_audio_update(odroid_ui_entry *entry) {
    if (*gAudioEnabledPointer) {
        sprintf(entry->text, "%-9s: %s", "audio", "on");
    } else {
        sprintf(entry->text, "%-9s: %s", "audio", "off");
    }
}

odroid_ui_func_toggle_rc menu_lynx_audio_toggle(odroid_ui_entry *entry, odroid_gamepad_state *joystick) {
    *gAudioEnabledPointer = !(*gAudioEnabledPointer);
    return ODROID_UI_FUNC_TOGGLE_RC_CHANGED;
}

#define FRAMESKIP_MAX 6
uint8_t frameskip = 2;

void menu_lynx_frameskip_update(odroid_ui_entry *entry) {
    sprintf(entry->text, "%-9s: %d", "frameskip", frameskip - 1);
}

odroid_ui_func_toggle_rc menu_lynx_frameskip_toggle(odroid_ui_entry *entry, odroid_gamepad_state *joystick) {
    if (joystick->values[ODROID_INPUT_A] || joystick->values[ODROID_INPUT_RIGHT]) {
        if (frameskip<FRAMESKIP_MAX) frameskip++;
    } else if (joystick->values[ODROID_INPUT_LEFT]) {
        if (frameskip>2) frameskip--;
    }
    return ODROID_UI_FUNC_TOGGLE_RC_CHANGED;
}

void menu_lynx_rotate_update(odroid_ui_entry *entry) {
    switch (rotate)
    {
    case 0: sprintf(entry->text, "%-9s: %s", "rotate", "off"); break;
    case 1: sprintf(entry->text, "%-9s: %s", "rotate", "right"); break;
    case 2: sprintf(entry->text, "%-9s: %s", "rotate", "left"); break;
    }
}

odroid_ui_func_toggle_rc menu_lynx_rotate_toggle(odroid_ui_entry *entry, odroid_gamepad_state *joystick) {
    rotate = (rotate+1)%3;
    return ODROID_UI_FUNC_TOGGLE_RC_CHANGED;
}

void menu_lynx_filtering_update(odroid_ui_entry *entry) {
    switch(filtering) {
    case 0:
        sprintf(entry->text, "%-9s: %s", "filter", "none");
        break;
    case 1:
        sprintf(entry->text, "%-9s: %s", "filter", "H");
        break;
    case 2:
        sprintf(entry->text, "%-9s: %s", "filter", "V");
        break;
    case 3:
        sprintf(entry->text, "%-9s: %s", "filter", "HV");
        break;
    }
}

odroid_ui_func_toggle_rc menu_lynx_filtering_toggle(odroid_ui_entry *entry, odroid_gamepad_state *joystick) {
    if (joystick->values[ODROID_INPUT_A] || joystick->values[ODROID_INPUT_RIGHT]) {
        if (++filtering>3) filtering = 0;
    } else if (joystick->values[ODROID_INPUT_LEFT]) {
        if (filtering--==0) filtering = 3;
    }
    return ODROID_UI_FUNC_TOGGLE_RC_CHANGED;
}

void menu_lynx_init(odroid_ui_window *window) {
    odroid_ui_create_entry(window, &menu_lynx_audio_update, &menu_lynx_audio_toggle);
    odroid_ui_create_entry(window, &menu_lynx_rotate_update, &menu_lynx_rotate_toggle);
    odroid_ui_create_entry(window, &menu_lynx_filtering_update, &menu_lynx_filtering_toggle);
    odroid_ui_create_entry(window, &menu_lynx_frameskip_update, &menu_lynx_frameskip_toggle);
}

inline void update_ui_fps() {
	stopTime = xthal_get_ccount();
    int elapsedTime;
    if (stopTime > startTime)
      elapsedTime = (stopTime - startTime);
    else
      elapsedTime = ((uint64_t)stopTime + (uint64_t)0xffffffff) - (startTime);

    totalElapsedTime += elapsedTime;
    ++frame;

    if (frame == 60)
    {
      float seconds = totalElapsedTime / (CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ * 1000000.0f);
      float fps = frame / seconds;
      fps_ui = fps;
      printf("FPS:%f, BATTERY:%d [%d]\n", fps, battery.millivolts, battery.percentage);

      //printf("HEAP:0x%x, FPS:%f, BATTERY:%d [%d]\n", esp_get_free_heap_size(), fps, battery.millivolts, battery.percentage);
      
      //vTaskGetRunTimeStats(pmem);
      //printf(pmem);
      
      frame = 0;
      totalElapsedTime = 0;
      /*if (config_ui_stats) {
        update_ui_fps_text(fps);
      }*/
#ifdef ODROID_DEBUG_PERF_USE
    odroid_debug_perf_log_specific(ODROID_DEBUG_PERF_TOTAL, 269334479);
    odroid_debug_perf_log_specific(ODROID_DEBUG_PERF_CPU, 230473555);
    odroid_debug_perf_log_specific(ODROID_DEBUG_PERF_SUSIE_PAINTSPRITES, 144456221);
    odroid_debug_perf_log_specific(ODROID_DEBUG_PERF_SUSIE_PAINTSPRITES_VLOOP, 135921297);
#endif
ODROID_DEBUG_PERF_LOG()
    }
    startTime = stopTime;
}


void odroid_retro_log(retro_log_level level,
      const char *fmt, ...) {
	va_list(args);
	switch(level) {
	case RETRO_LOG_DEBUG:
	  printf("DEBUG - ");
      break;
    case RETRO_LOG_INFO:
      printf("INFO  - ");
      break;
    case RETRO_LOG_WARN:
      printf("WARN  - ");
      break;
    case RETRO_LOG_ERROR:
      printf("ERROR - ");
      break;
    default:
      printf("LOG%d - ", level);
    break;
	};
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");
}

//expected 'retro_environment_t {aka _Bool (*)(unsigned int,  void *)}'
//but argument is of type '_Bool (*)(unsigned int,  void **)' RETRO_API void retro_set_environment(retro_environment_t);

bool odroidgo_env(unsigned int cmd, void *_data) {
   void **data = (void**)_data;
   if (!data) {
     printf("CMD '%d'. Data is null!\n", cmd);
   }
   switch(cmd) {
   case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
   {
    retro_log_callback *log = (retro_log_callback *)data;
   	log->log = &odroid_retro_log;
   }
   break;
   case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
    *data = &odroidgo_fmt;
    break;
   case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
    *data = (char*)odroidgo_rom_path;
   break;
   case RETRO_ENVIRONMENT_SET_VARIABLES:
     // VARs from lynx emu
   break;
   case RETRO_ENVIRONMENT_SET_SERIALIZATION_QUIRKS:
      // serialization_quirks
   break;
   case RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS:
     //
   break;
   case RETRO_ENVIRONMENT_GET_VARIABLE:
     *data = NULL;
     return false;
   case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE:
     return false;
   default:
    data = NULL;
    printf("No data for '%d'!\n", cmd);
    abort();
   return false;
   };
   return true;
}

void odroid_retro_input_poll_t() {
   //
}

void odroid_retro_audio_sample_t(int16_t left, int16_t right) {
   printf("odroid_retro_audio_sample_t\n");
}

size_t odroid_retro_audio_sample_batch_t(const int16_t *data, size_t frames) {
   // Process audio
#ifdef MY_AUDIO_MODE_V1
    //printf("Audio: Frames: %u\n", frames);
    audio_update->length = frames;
    // xQueueSend(audioQueue, &audio_update, portMAX_DELAY);
    // *** odroid_audio_submit(audio_update->buffer, audio_update->length - 1);
    
    odroid_audio_submit(audio_update->buffer, audio_update->length - 1);
    
    audio_update = (audio_update==&audio_update1)?&audio_update2:&audio_update1;
    gAudioBuffer = audio_update->buffer;
#else
    odroid_audio_submit((short*)data, frames - 1);
#endif
   return 0;
}

    uint16_t menuButtonFrameCount;
    odroid_gamepad_state previousState;
    bool ignoreMenuButton, menu_restart;
    
bool odroid_ui_menu_lynx(bool mr)
{
    mr = odroid_ui_menu_ext(mr, &menu_lynx_init);
    if (previous_scaling_enabled != scaling_enabled ||
        previous_filtering != filtering ||
        previous_rotate != rotate) {
      if (!mr)
      {
        update_display_task();
      }
    
      // display_func_change = true;
      //odroid_display_lock();
      //ili9341_write_frame_lynx(NULL, NULL, false);
      //update_display_func();
      //odroid_display_unlock();
    }
    return mr;
}
    
void process_keys(odroid_gamepad_state *joystick)
{
#ifndef MY_KEYS_IN_CALLBACK
        retrolib_input_state_t = 0;
        if (joystick->values[ODROID_INPUT_A]) {
                retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_A;
        }
        if (joystick->values[ODROID_INPUT_B]) {
                retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_B;
        }
        if (joystick->values[ODROID_INPUT_LEFT]) {
                retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_LEFT;
        }
        if (joystick->values[ODROID_INPUT_RIGHT]) {
                retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_RIGHT;
        }
        if (joystick->values[ODROID_INPUT_UP]) {
                retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_UP;
        }
        if (joystick->values[ODROID_INPUT_DOWN]) {
                retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_DOWN;
        }
        if (joystick->values[ODROID_INPUT_START]) {
                retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_START;
        }
        if (joystick->values[ODROID_INPUT_SELECT]) {
                retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_L;
        }
        // { RETRO_DEVICE_ID_JOYPAD_R, BUTTON_OPT2 },
#endif
        ODROID_UI_MENU_HANDLER_LOOP_V1(previousState, (*joystick), DoMenuHome, odroid_ui_menu_lynx)
}

#ifdef MY_KEYS_IN_CALLBACK
int16_t odroid_retro_input_state_t(unsigned port, unsigned device, 
      unsigned index, unsigned id) {
      int16_t rc;
      switch(id) {
        case RETRO_DEVICE_ID_JOYPAD_A:
            rc = previousState.values[ODROID_INPUT_A];
            break;
        case RETRO_DEVICE_ID_JOYPAD_B:
            rc = previousState.values[ODROID_INPUT_B];
            break;
        case RETRO_DEVICE_ID_JOYPAD_LEFT:
            rc = previousState.values[ODROID_INPUT_LEFT];
            break;
        case RETRO_DEVICE_ID_JOYPAD_RIGHT:
            rc = previousState.values[ODROID_INPUT_RIGHT];
            break;
        case RETRO_DEVICE_ID_JOYPAD_UP:
            rc = previousState.values[ODROID_INPUT_UP];
            break;
        case RETRO_DEVICE_ID_JOYPAD_DOWN:
            rc = previousState.values[ODROID_INPUT_DOWN];
            break;
        case RETRO_DEVICE_ID_JOYPAD_START:
            rc = previousState.values[ODROID_INPUT_START];
            break;
        case RETRO_DEVICE_ID_JOYPAD_L:
            rc = previousState.values[ODROID_INPUT_SELECT];
            break;
        // { RETRO_DEVICE_ID_JOYPAD_R, BUTTON_OPT2 },
        default:
            rc = 0;
            break;
      }
      return rc;
}
#else
int16_t odroid_retro_input_state_t(unsigned port, unsigned device, 
      unsigned index, unsigned id) {
      return (1<<id) & retrolib_input_state_t;
      //return retrolib_input_state_t;
      //return ((retrolib_input_state_t&0x00ff) << 8) | ((retrolib_input_state_t&0xff00) >> 8);
}
#endif



bool skipNextFrame = true;
void odroid_retro_video_refresh_t(const void *data) {
	 if ((frame%frameskip)==1) {
     	xQueueSend(vidQueue, &data, portMAX_DELAY);
     	skipNextFrame = true;
     } else if ((frame%frameskip)==0) {
     	skipNextFrame = false;
     } else {
     	skipNextFrame = true;
     }
     update_ui_fps();

#ifdef MY_KEYS_IN_VIDEO
        odroid_gamepad_state joystick;   
        odroid_input_gamepad_read(&joystick);
        
        process_keys(&joystick);
        previousState = joystick;
#endif
}

void odroidgo_retro_init(void) {
	printf("odroidgo_init\n");
	odroid_display_unlock();
	char *rc = odroid_ui_choose_file("/sd/roms/lynx", "lnx");
	odroid_display_lock();
	if (rc) {
	   printf("File: %s\n", rc);
	   odroid_game.path = rc;
	} else {
	   printf("File: ---\n");
	   odroid_game.path = "/sd/roms/lynx/_debug.lnx";
	}
	
	odroid_game.data = NULL;
    odroid_game.size = 0;
    odroid_game.meta = "dummy";
	
	retro_set_environment(&odroidgo_env);
	retro_set_audio_sample(&odroid_retro_audio_sample_t);
	retro_set_video_refresh(&odroid_retro_video_refresh_t);
	retro_set_audio_sample_batch(&odroid_retro_audio_sample_batch_t);
	// retro_set_controller_port_device();
}

void odroidgo_retro_init_post() {
	retro_set_input_poll(&odroid_retro_input_poll_t);
	retro_set_input_state(&odroid_retro_input_state_t);
}

/*
#include "esp_heap_trace.h"
#define NUM_RECORDS 1000
static heap_trace_record_t trace_record[NUM_RECORDS]; // This buffer must be in internal RAM
*/

void dump_heap_info_short() {
    printf("LARGEST: 8BIT: %u\n", heap_caps_get_largest_free_block( MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT ));
    printf("LARGEST: 32BIT: %u\n", heap_caps_get_largest_free_block( MALLOC_CAP_INTERNAL | MALLOC_CAP_32BIT ));
    printf("LARGEST: MALLOC_CAP_INTERNAL: %u\n", heap_caps_get_largest_free_block( MALLOC_CAP_INTERNAL ));
    printf("LARGEST: MALLOC_CAP_DEFAULT: %u\n", heap_caps_get_largest_free_block( MALLOC_CAP_DEFAULT ));
}

void app_loop(void)
{
    menuButtonFrameCount = 0;
    menu_restart = false;
    odroid_input_gamepad_read(&previousState);
    ignoreMenuButton = previousState.values[ODROID_INPUT_MENU];
    
#ifdef MY_RETRO_LOOP
    retro_run_endless();
#else
    while (true)
    {
#ifndef MY_KEYS_IN_VIDEO
        odroid_gamepad_state joystick;   
        odroid_input_gamepad_read(&joystick);
        process_keys(&joystick);
#endif
        retro_run();
#ifndef MY_KEYS_IN_VIDEO
        previousState = joystick;
#endif
    }
#endif
}

//#define VID_BUF_SIZE (160*102*2)
#define VID_BUF_SIZE ((160/2+64)*102)

NOINLINE void app_init(void)
{
printf("lynx-handy (%s-%s).\n", COMPILEDATE, GITREV);
    // ESP_ERROR_CHECK( heap_trace_init_standalone(trace_record, NUM_RECORDS) );
    
    framebuffer[0] = heap_caps_malloc(VID_BUF_SIZE, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    //framebuffer[0] = MY_MEM_ALLOC_SLOW_EXT(uint16_t, VID_BUF_SIZE, 1); // slower
    if (!framebuffer[0]) abort();
    printf("app_main: framebuffer[0]=%p\n", framebuffer[0]);

    framebuffer[1] = heap_caps_malloc(VID_BUF_SIZE, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    //framebuffer[1] = MY_MEM_ALLOC_SLOW_EXT(uint16_t, VID_BUF_SIZE, 1); // slower
    if (!framebuffer[1]) abort();
    printf("app_main: framebuffer[1]=%p\n", framebuffer[1]);


#ifdef MY_AUDIO_MODE_V1
    audio_update1.buffer = MY_MEM_ALLOC_SLOW_EXT(short, AUDIO_BUFFER_SIZE, 1);
    //audio_update1.buffer = heap_caps_malloc(AUDIO_BUFFER_SIZE, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL); // slower?
    if (!audio_update1.buffer) abort();

    audio_update2.buffer = MY_MEM_ALLOC_SLOW_EXT(short, AUDIO_BUFFER_SIZE, 1);
    //audio_update2.buffer = heap_caps_malloc(AUDIO_BUFFER_SIZE, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL); // slower?
    if (!audio_update2.buffer) abort();

    gAudioBuffer = audio_update1.buffer;
    gAudioBufferPointer2 = gAudioBuffer;
#endif
    
    QuickSaveSetBuffer( MY_MEM_ALLOC_SLOW(void, 512*1024) );
    
    // ESP_ERROR_CHECK( heap_trace_start(HEAP_TRACE_LEAKS) );
    
    nvs_flash_init();

    odroid_system_init();

    // Joystick.
    odroid_input_gamepad_init();
    odroid_input_battery_level_init();
    ili9341_prepare();

    // Disable LCD CD to prevent garbage
    const gpio_num_t LCD_PIN_NUM_CS = GPIO_NUM_5;

    gpio_config_t io_conf = { 0 };
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << LCD_PIN_NUM_CS);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;

    gpio_config(&io_conf);
    gpio_set_level(LCD_PIN_NUM_CS, 1);

    check_boot_cause();

    ili9341_init();
    
    char* cartName = odroid_settings_RomFilePath_get();

    ili9341_write_frame_lynx(NULL, NULL, false);

    odroid_audio_init(odroid_settings_AudioSink_get(), AUDIO_SAMPLE_RATE);

    vidQueue = xQueueCreate(1, sizeof(uint16_t*));
    //xTaskCreatePinnedToCore(&videoTask, "videoTask", 1024 * 4, NULL, 5, &videoTaskHandle, 1);
#ifdef MY_AUDIO_MODE_V1
    audioQueue = xQueueCreate(1, sizeof(uint16_t*));
    xTaskCreatePinnedToCore(&audioTask, "audioTask", 2048, NULL, 5, NULL, 1); //768    
#endif

    esp_err_t r = odroid_sdcard_open(SD_BASE_PATH);
    if (r != ESP_OK)
    {
        odroid_display_show_sderr(ODROID_SD_ERR_NOCARD);
        abort();
    }
        
    odroid_display_lock();
    odroid_display_drain_spi();
    
    // ESP_ERROR_CHECK( heap_trace_start(HEAP_TRACE_LEAKS) );
    printf("LYNX-hande: 001\n");
    printf("Version: %d; %d\n", RETRO_API_VERSION, RETRO_MEMORY_VIDEO_RAM);
    odroidgo_retro_init();
    retro_init();
    printf("LYNX-hande: 002\n");
    
    printf("Retro: API: %d\n", retro_api_version());
    retro_get_system_info(&retro_info);
    printf("Retro: Info.library_name      : %s\n", retro_info.library_name);
    printf("Retro: Info.library_version   : %s\n", retro_info.library_version);
    printf("Retro: Info.need_fullpath     : %d\n", retro_info.need_fullpath);
    printf("Retro: Info.valid_extensions  : %s\n", retro_info.valid_extensions);
    printf("Retro: Info.block_extract     : %d\n", retro_info.block_extract);
    
    odroid_display_unlock();
    
    if (!retro_load_game(&odroid_game)) {
        printf("LYNX-handy: 003 Gameload: Error\n");
    } else {
       printf("LYNX-handy: 003 Gameload: Ok\n");
    }
    
    printf("LYNX-handy: 003\n");
    
    // FIXME: Do some EMU stuff
    // system_reset();

    // Restore state
    //LoadState(cartName);

    if (forceConsoleReset)
    {
        // Reset emulator if button held at startup to
        // override save state
        printf("%s: forceConsoleReset=true\n", __func__);
        // system_reset();
    }
    odroidgo_retro_init_post();
    
    // ESP_ERROR_CHECK( heap_trace_stop() );
    // heap_trace_dump();

    odroid_gamepad_state previousState;
    odroid_input_gamepad_read(&previousState);

    totalElapsedTime = 0;
    frame = 0;
    scaling_enabled = odroid_settings_ScaleDisabled_get(ODROID_SCALE_DISABLE_SMS) ? false : true;
    
    odroid_ui_enter_loop();
#ifdef ODROID_DEBUG_PERF_USE
    printf("heap_caps metadata test\n");
    heap_caps_print_heap_info(MALLOC_CAP_8BIT);
    heap_caps_print_heap_info(MALLOC_CAP_32BIT);
    heap_caps_print_heap_info(MALLOC_CAP_INTERNAL);
#endif
    
    startTime = xthal_get_ccount();
    
    dump_heap_info_short();
    printf("unsigned char   : %u\n", sizeof(unsigned char));
    printf("unsigned int    : %u\n", sizeof(unsigned int));
    printf("unsigned long   : %u\n", sizeof(unsigned long));
    
    /*
    uint32_t caps = MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT;
    for (int i = 0; i < 64; i++) {
    void *tmp = heap_caps_malloc(1024, caps);// MALLOC_CAP_SPIRAM
    if (!tmp) {
        printf("PRE-ALLOC failed #%d\n", i+1);
        odroid_ui_debug_enter_loop();
        printf("LARGEST: %u\n", heap_caps_get_largest_free_block( caps ));
        abort();
    }
        printf("PRE-ALLOC test #%d: OK: %p\n", i+1, tmp);
        printf("LARGEST: %u\n", heap_caps_get_largest_free_block( caps ));
    }
    */
    if (!forceConsoleReset)
    {
       LoadState(cartName);
    }
    
    update_display_task();
}

void app_main(void)
{
    app_init();
    app_loop();
}

void *my_special_alloc(unsigned char speed, unsigned char bytes, unsigned long size) {
    // if (size==16384) speed = 1;
    
    uint32_t caps = (speed?MALLOC_CAP_INTERNAL:MALLOC_CAP_SPIRAM) | 
      ( bytes==1?MALLOC_CAP_8BIT:MALLOC_CAP_32BIT);
      /*
    if (speed) {
        uint32_t max = heap_caps_get_largest_free_block(caps);
        if (max < size) {
            printf("ALLOC: Size: %u; Max FREE for internal is '%u'. Allocating in SPI RAM\n", (unsigned int)size, max);
            caps = MALLOC_CAP_SPIRAM | ( size==1?MALLOC_CAP_8BIT:MALLOC_CAP_32BIT);
        }
    } else {
      caps = MALLOC_CAP_SPIRAM | MALLOC_CAP_32BIT;
    }
    */
    if (!speed) caps = MALLOC_CAP_SPIRAM | MALLOC_CAP_32BIT;
    //if (!speed || size!=65536) caps = MALLOC_CAP_SPIRAM | MALLOC_CAP_32BIT; // only RAM
    void *rc = heap_caps_malloc(size, caps);
    printf("ALLOC: Size: %-10u; SPI: %u; 32BIT: %u; RC: %p\n", (unsigned int)size, (caps&MALLOC_CAP_SPIRAM)!=0, (caps&MALLOC_CAP_32BIT)!=0, rc);
    if (!rc) { dump_heap_info_short(); abort(); }
    return rc;
}

void my_special_alloc_free(void *p) {
    printf("FREE: Size: -; RC: %p\n", p);
    if (p) heap_caps_free(p);
}
