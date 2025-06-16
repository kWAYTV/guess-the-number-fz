#include "boilerplate_speaker.h"
#include "../boilerplate.h"

#define NOTE_INPUT   587.33f
#define NOTE_SUCCESS 880.0f // A5 - Higher pitched for success
#define NOTE_ERROR   293.66f // D4 - Lower pitched for errors
#define NOTE_CLOSE   659.25f // E5 - Mid pitched for close guesses

void boilerplate_play_input_sound(void* context) {
    Boilerplate* app = context;
    if(app->speaker != 1) {
        return;
    }
    float volume = 1.0f;
    if(furi_hal_speaker_is_mine() || furi_hal_speaker_acquire(30)) {
        furi_hal_speaker_start(NOTE_INPUT, volume);
    }
}

void boilerplate_play_win_sound(void* context) {
    Boilerplate* app = context;
    if(app->speaker != 1) {
        return;
    }
    float volume = 1.0f;
    if(furi_hal_speaker_is_mine() || furi_hal_speaker_acquire(30)) {
        // Play a victory melody: high note followed by even higher note
        furi_hal_speaker_start(NOTE_SUCCESS, volume);
        furi_thread_flags_wait(0, FuriFlagWaitAny, 150);
        furi_hal_speaker_start(NOTE_SUCCESS * 1.5f, volume);
    }
}

void boilerplate_play_close_sound(void* context) {
    Boilerplate* app = context;
    if(app->speaker != 1) {
        return;
    }
    float volume = 0.8f;
    if(furi_hal_speaker_is_mine() || furi_hal_speaker_acquire(30)) {
        furi_hal_speaker_start(NOTE_CLOSE, volume);
    }
}

void boilerplate_play_error_sound(void* context) {
    Boilerplate* app = context;
    if(app->speaker != 1) {
        return;
    }
    float volume = 0.6f;
    if(furi_hal_speaker_is_mine() || furi_hal_speaker_acquire(30)) {
        furi_hal_speaker_start(NOTE_ERROR, volume);
    }
}

void boilerplate_stop_all_sound(void* context) {
    Boilerplate* app = context;
    if(app->speaker != 1) {
        return;
    }
    if(furi_hal_speaker_is_mine()) {
        furi_hal_speaker_stop();
        furi_hal_speaker_release();
    }
}
