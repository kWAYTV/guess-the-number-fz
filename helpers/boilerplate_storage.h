#pragma once

#include <stdlib.h>
#include <string.h>
#include <storage/storage.h>
#include <flipper_format/flipper_format_i.h>
#include "../boilerplate.h"

#define BOILERPLATE_SETTINGS_FILE_VERSION      1
#define CONFIG_FILE_DIRECTORY_PATH             EXT_PATH("apps_data/guess_the_number")
#define BOILERPLATE_SETTINGS_SAVE_PATH         CONFIG_FILE_DIRECTORY_PATH "/guess_the_number.conf"
#define BOILERPLATE_SETTINGS_SAVE_PATH_TMP     BOILERPLATE_SETTINGS_SAVE_PATH ".tmp"
#define BOILERPLATE_SETTINGS_HEADER            "GTN Config File"
#define BOILERPLATE_SETTINGS_KEY_HAPTIC        "Haptic"
#define BOILERPLATE_SETTINGS_KEY_LED           "Led"
#define BOILERPLATE_SETTINGS_KEY_SPEAKER       "Speaker"
#define BOILERPLATE_SETTINGS_KEY_SAVE_SETTINGS "SaveSettings"

// Save game preferences (haptic, sound, LED settings) to persistent storage
void boilerplate_save_settings(void* context);
// Load game preferences from persistent storage, using defaults if file doesn't exist
void boilerplate_read_settings(void* context);
