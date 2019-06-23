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

#include "../components/odroid/odroid_settings.h"
#include "../components/odroid/odroid_audio.h"
#include "../components/odroid/odroid_input.h"
#include "../components/odroid/odroid_system.h"
#include "../components/odroid/odroid_display.h"
#include "../components/odroid/odroid_sdcard.h"
#include "../components/odroid/odroid_ui.h"

#include <dirent.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
2019-06-17 with sound									HEAP:0x35e1f8, FPS:34.789825, BATTERY:4156 [93]
2019-06-17 ohne audio									HEAP:0x35e204, FPS:34.785297, BATTERY:4157 [93]
2019-06-17 ohne audio, frameskip: 10						HEAP:0x35e204, FPS:44.046673, BATTERY:4159 [94]
2019-06-17 ohne audio, frameskip: 1						HEAP:0x35e204, FPS:41.796509, BATTERY:4158 [94]
2019-06-17 ohne audio, frameskip: 1, mikie: 10th line		HEAP:0x35e200, FPS:45.753841, BATTERY:4159 [94]
2019-06-17 ohne audio, frameskip: 2, mikie: -			HEAP:0x35e204, FPS:44.118404, BATTERY:4155 [93]
2019-06-17 ohne audio, frameskip: 2, mikie: +			HEAP:0x35e204, FPS:46.717072, BATTERY:4156 [93]
2019-06-17 OHNE audio, frameskip: 2, mikie: +			HEAP:0x35e204, FPS:49.060566, BATTERY:4157 [93]
2019-06-17 with audio, frameskip: 2, mikie: +			HEAP:0x35e1f8, FPS:44.029408, BATTERY:4157 [93]
*/

#define PALETTE_SIZE 256

const char* SD_BASE_PATH = "/sd";

//#define AUDIO_SAMPLE_RATE (32000)
#define AUDIO_SAMPLE_RATE (22050)

uint16 palette[PALETTE_SIZE];
uint16_t* framebuffer[2]; // uint8_t*
int currentFramebuffer = 0;

uint32_t* audioBuffer = NULL;
int audioBufferCount = 0;

spi_flash_mmap_handle_t hrom;

QueueHandle_t vidQueue;
TaskHandle_t videoTaskHandle;

odroid_volume_level Volume;
odroid_battery_state battery;

bool scaling_enabled = true;
bool previous_scaling_enabled = true;

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

void videoTask(void *arg)
{
    uint8_t* param;

    videoTaskIsRunning = true;

    while(1)
    {
        xQueuePeek(vidQueue, &param, portMAX_DELAY);

        if (param == 1)
            break;

        if (previous_scaling_enabled != scaling_enabled)
        {
            ili9341_write_frame_lynx(NULL, NULL, false);
            previous_scaling_enabled = scaling_enabled;
        }
        //render_copy_palette(palette);
        ili9341_write_frame_lynx(param, palette, scaling_enabled);

        odroid_input_battery_level_read(&battery);

        xQueueReceive(vidQueue, &param, portMAX_DELAY);
    }

    odroid_display_lock();

    // Draw hourglass
    odroid_display_show_hourglass();

    odroid_display_unlock();

    videoTaskIsRunning = false;
    vTaskDelete(NULL);

    while (1) {}
}

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

const char* StateFileName = "/storage/smsplus.sav";
const char* StoragePath = "/storage";

static void SaveState()
{
}

static void LoadState(const char* cartName)
{
}

bool DoSaveState(const char* pathName) {
    return true;
}

bool DoLoadState(const char* pathName) {
    return true;
}

bool QuickSaveState(FILE* f)
{
    return true;
}

bool QuickLoadState(FILE* f)
{
    return true;
}

static void PowerDown()
{
    uint16_t* param = 1;

    // Clear audio to prevent studdering
    printf("PowerDown: stopping audio.\n");
    odroid_audio_terminate();

    // Stop tasks
    printf("PowerDown: stopping tasks.\n");

    xQueueSend(vidQueue, &param, portMAX_DELAY);
    while (videoTaskIsRunning) { vTaskDelay(1); }


    // state
    printf("PowerDown: Saving state.\n");
    SaveState();

    // LCD
    printf("PowerDown: Powerdown LCD panel.\n");
    ili9341_poweroff();

    odroid_system_sleep();

    // Should never reach here
    abort();
}

static void DoHome()
{
    esp_err_t err;
    uint16_t* param = 1;

    // Clear audio to prevent studdering
    printf("PowerDown: stopping audio.\n");
    odroid_audio_terminate();


    // Stop tasks
    printf("PowerDown: stopping tasks.\n");

    xQueueSend(vidQueue, &param, portMAX_DELAY);
    while (videoTaskIsRunning) { vTaskDelay(1); }


    // state
    printf("PowerDown: Saving state.\n");
    SaveState();


    // Set menu application
    odroid_system_application_set(0);


    // Reset
    esp_restart();
}
/*
void system_manage_sram(uint8 *sram, int slot, int mode)
{
    printf("system_manage_sram\n");
    //sram_load();
}
*/

inline void update_fps() {
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

      printf("HEAP:0x%x, FPS:%f, BATTERY:%d [%d]\n", esp_get_free_heap_size(), fps, battery.millivolts, battery.percentage);

      frame = 0;
      totalElapsedTime = 0;
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
	va_end(fmt);
	printf("\n");
}

bool odroidgo_env(unsigned cmd, void **data) {
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
    *data = odroidgo_rom_path;
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

int16_t odroid_retro_input_state_t(unsigned port, unsigned device, 
      unsigned index, unsigned id) {
      return (1<<id) & retrolib_input_state_t;
      //return retrolib_input_state_t;
      //return ((retrolib_input_state_t&0x00ff) << 8) | ((retrolib_input_state_t&0xff00) >> 8);
}

void odroid_retro_input_poll_t() {
   //
}

void odroid_retro_audio_sample_t(int16_t left, int16_t right) {
   printf("odroid_retro_audio_sample_t\n");
}

size_t odroid_retro_audio_sample_batch_t(const int16_t *data, size_t frames) {
   // Process audio
   /*
    for (int x = 0; x < frames; x+=2)
    {
        uint32_t sample;
        // sample = (  ( ((data[x]&0x00ff) << 8) | ((data[x]&0xff00) >> 8) )  << 16);
        //sample = ((data[x]&0xff00) << 16);
        sample = (data[x] << 16);
        audioBuffer[x/2] = sample;
    }
    odroid_audio_submit((short*)audioBuffer, frames/2 - 1);
    */
	// send audio
    memcpy(audioBuffer, data, frames*4);
	odroid_audio_submit((short*)audioBuffer, frames - 1);
   return 0;
}

bool skipNextFrame = true;

void odroid_retro_video_refresh_t(const void *data, unsigned width,
      unsigned height, size_t pitch) {
     
	 if ((frame%3)==1) {
	 	memcpy(framebuffer[currentFramebuffer], data, height*pitch);
	  
	 	xQueueSend(vidQueue, &framebuffer[currentFramebuffer], portMAX_DELAY);
     	currentFramebuffer = currentFramebuffer ? 0 : 1;
     	skipNextFrame = true;
     } else if ((frame%3)==0) {
         // printf("Draw frame: %d, %d\n", frame, frame%4);
     	skipNextFrame = false;
     } else {
     	skipNextFrame = true;
     }
     update_fps();
}

void odroidgo_retro_init(void) {
	printf("odroidgo_init\n");
	odroid_game.path = "/sd/roms/lynx/_debug.lnx";
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

extern uint32    gAudioEnabled;

void menu_lynx_audio_update(odroid_ui_entry *entry) {
    if (gAudioEnabled) {
        sprintf(entry->text, "%-9s: %s", "audio", "on");
    } else {
        sprintf(entry->text, "%-9s: %s", "audio", "off");
    }
}

odroid_ui_func_toggle_rc menu_lynx_audio_toggle(odroid_ui_entry *entry, odroid_gamepad_state *joystick) {
    gAudioEnabled = !gAudioEnabled;
    return ODROID_UI_FUNC_TOGGLE_RC_CHANGED;
}

void menu_lynx_init(odroid_ui_window *window) {
    odroid_ui_create_entry(window, &menu_lynx_audio_update, &menu_lynx_audio_toggle);
}

void app_main(void)
{
    printf("lynx-handy (%s-%s).\n", COMPILEDATE, GITREV);
	
    framebuffer[0] = heap_caps_malloc(160 * 102 * 2, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    if (!framebuffer[0]) abort();
    printf("app_main: framebuffer[0]=%p\n", framebuffer[0]);

    framebuffer[1] = heap_caps_malloc(160 * 102 * 2, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    if (!framebuffer[1]) abort();
    printf("app_main: framebuffer[1]=%p\n", framebuffer[1]);
	
    size_t bufferSize = 4096 * 2 * sizeof(int16_t);
    audioBuffer = heap_caps_malloc(bufferSize, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    if (!audioBuffer) abort();

    nvs_flash_init();

    odroid_system_init();

    // Joystick.
    odroid_input_gamepad_init();
    odroid_input_battery_level_init();


    // Boot state overrides
    bool forceConsoleReset = false;


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


    switch (esp_sleep_get_wakeup_cause())
    {
        case ESP_SLEEP_WAKEUP_EXT0:
        {
            printf("app_main: ESP_SLEEP_WAKEUP_EXT0 deep sleep reset\n");
            break;
        }

        case ESP_SLEEP_WAKEUP_EXT1:
        case ESP_SLEEP_WAKEUP_TIMER:
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
        case ESP_SLEEP_WAKEUP_ULP:
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        {
            printf("app_main: Unexpected deep sleep reset\n");

            odroid_gamepad_state bootState = odroid_input_read_raw();

            if (bootState.values[ODROID_INPUT_MENU])
            {
                // Force return to factory app to recover from
                // ROM loading crashes

                // Set menu application
                odroid_system_application_set(0);

                // Reset
                esp_restart();
            }

            if (bootState.values[ODROID_INPUT_START])
            {
                // Reset emulator if button held at startup to
                // override save state
                forceConsoleReset = true; //emu_reset();
            }
        }
            break;

        default:
            printf("app_main: Not a deep sleep reset\n");
            break;
    }

    if (odroid_settings_StartAction_get() == ODROID_START_ACTION_RESTART)
    {
        forceConsoleReset = true;
        odroid_settings_StartAction_set(ODROID_START_ACTION_NORMAL);
    }


    ili9341_init();
    
    char* cartName = odroid_settings_RomFilePath_get();

    ili9341_write_frame_lynx(NULL, NULL, false);

    odroid_audio_init(odroid_settings_AudioSink_get(), AUDIO_SAMPLE_RATE);

    vidQueue = xQueueCreate(1, sizeof(uint16_t*));
    xTaskCreatePinnedToCore(&videoTask, "videoTask", 1024 * 4, NULL, 5, &videoTaskHandle, 1);
    
    esp_err_t r = odroid_sdcard_open(SD_BASE_PATH);
    if (r != ESP_OK)
    {
        odroid_display_show_sderr(ODROID_SD_ERR_NOCARD);
        abort();
    }
        
    odroid_display_lock();
    odroid_display_drain_spi();
    
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
    LoadState(cartName);

    if (forceConsoleReset)
    {
        // Reset emulator if button held at startup to
        // override save state
        printf("%s: forceConsoleReset=true\n", __func__);
        // system_reset();
    }
	odroidgo_retro_init_post();

    odroid_gamepad_state previousState;
    odroid_input_gamepad_read(&previousState);

    totalElapsedTime = 0;
    frame = 0;
    uint16_t muteFrameCount = 0;
    uint16_t powerFrameCount = 0;

    bool ignoreMenuButton = previousState.values[ODROID_INPUT_MENU];

    scaling_enabled = odroid_settings_ScaleDisabled_get(ODROID_SCALE_DISABLE_SMS) ? false : true;
    
    odroid_ui_debug_enter_loop();
    startTime = xthal_get_ccount();
    
    bool menu_restart = false;

    while (true)
    {
        odroid_gamepad_state joystick;
        odroid_input_gamepad_read(&joystick);
        
        retrolib_input_state_t = 0;
        if (joystick.values[ODROID_INPUT_A]) {
        		retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_A;
        }
        if (joystick.values[ODROID_INPUT_B]) {
        		retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_B;
        }
        if (joystick.values[ODROID_INPUT_LEFT]) {
        		retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_LEFT;
        }
        if (joystick.values[ODROID_INPUT_RIGHT]) {
        		retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_RIGHT;
        }
        if (joystick.values[ODROID_INPUT_UP]) {
        		retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_UP;
        }
        if (joystick.values[ODROID_INPUT_DOWN]) {
        		retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_DOWN;
        }
        if (joystick.values[ODROID_INPUT_START]) {
        		retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_START;
        }
        if (joystick.values[ODROID_INPUT_SELECT]) {
        		retrolib_input_state_t |= 1 << RETRO_DEVICE_ID_JOYPAD_L;
        }
   		// { RETRO_DEVICE_ID_JOYPAD_R, BUTTON_OPT2 },        

        if (ignoreMenuButton)
        {
            ignoreMenuButton = previousState.values[ODROID_INPUT_MENU];
        }

        if (!ignoreMenuButton && previousState.values[ODROID_INPUT_MENU] && joystick.values[ODROID_INPUT_MENU])
        {
            ++powerFrameCount;
        }
        else
        {
            powerFrameCount = 0;
        }

        // Note: this will cause an exception on 2nd Core in Debug mode
        if (powerFrameCount > 60 * 2)
        {
            // Turn Blue LED on. Power state change turns it off
            odroid_system_led_set(1);
            PowerDown();
        }

        if (joystick.values[ODROID_INPUT_VOLUME] || menu_restart)
        {
            menu_restart = odroid_ui_menu_ext(menu_restart, &menu_lynx_init);
        }

        if (!ignoreMenuButton && previousState.values[ODROID_INPUT_MENU] && !joystick.values[ODROID_INPUT_MENU])
        {
            DoHome();
        }


        // Scaling
        if (joystick.values[ODROID_INPUT_START] && !previousState.values[ODROID_INPUT_RIGHT] && joystick.values[ODROID_INPUT_RIGHT])
        {
            scaling_enabled = !scaling_enabled;
            odroid_settings_ScaleDisabled_set(ODROID_SCALE_DISABLE_SMS, scaling_enabled ? 0 : 1);
        }

		retro_run();

        previousState = joystick;
    }
}
