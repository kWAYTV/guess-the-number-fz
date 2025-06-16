#include "../boilerplate.h"
#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_random.h>
#include <input/input.h>
#include <gui/elements.h>
#include <dolphin/dolphin.h>
#include "../helpers/boilerplate_haptic.h"
#include "../helpers/boilerplate_speaker.h"
#include "../helpers/boilerplate_led.h"

struct BoilerplateScene2 {
    View* view;
    BoilerplateScene2Callback callback;
    void* context;
};

typedef struct {
    char* buffer;
    int target_number, player_guess;
    char game_message[50];
    int attempts;
    int best_score;
    bool game_won;
} BoilerplateScene2Model;

void boilerplate_scene_2_set_callback(
    BoilerplateScene2* instance,
    BoilerplateScene2Callback callback,
    void* context) {
    furi_assert(instance);
    furi_assert(callback);
    instance->callback = callback;
    instance->context = context;
}

void boilerplate_scene_2_draw(Canvas* canvas, BoilerplateScene2Model* model) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    // Title - compact
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 0, AlignCenter, AlignTop, "GUESS THE NUMBER");
    
    // Current guess - prominently displayed
    canvas_set_font(canvas, FontPrimary);
    char guessBuffer[10];
    snprintf(guessBuffer, sizeof(guessBuffer), "[ %02d ]", model->player_guess);
    canvas_draw_str_aligned(canvas, 64, 15, AlignCenter, AlignTop, guessBuffer);
    
    // Game feedback message - main feedback area
    if(strlen(model->game_message) > 0) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 30, AlignCenter, AlignTop, model->game_message);
    }

    // Win state handling - different layout when won
    if(model->game_won) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 40, AlignCenter, AlignTop, "Press BACK for new game");
    } else {
        // Show range when playing
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 40, AlignCenter, AlignTop, "Range: 0-99");
    }

    // Bottom status line - always at bottom
    canvas_set_font(canvas, FontSecondary);
    char statusBuffer[32];
    if(model->best_score > 0) {
        snprintf(statusBuffer, sizeof(statusBuffer), "Tries: %d | Best: %d", 
                model->attempts, model->best_score);
    } else {
        snprintf(statusBuffer, sizeof(statusBuffer), "Tries: %d", model->attempts);
    }
    canvas_draw_str_aligned(canvas, 64, 54, AlignCenter, AlignTop, statusBuffer);
}

static void boilerplate_scene_2_model_init(BoilerplateScene2Model* const model) {
    // Set the target number to a random number between 0 and 99
    model->target_number = furi_hal_random_get() % 100;
    model->player_guess = 50; // Start in the middle
    model->attempts = 0;
    model->game_won = false;
    strcpy(model->game_message, "Make your first guess!");
    // Keep best_score from previous games
    dolphin_deed(DolphinDeedPluginGameStart);
}

bool boilerplate_scene_2_input(InputEvent* event, void* context) {
    furi_assert(context);
    BoilerplateScene2* instance = context;

    if(event->type == InputTypeLong && event->key == InputKeyBack) {
        with_view_model(
            instance->view,
            BoilerplateScene2Model * model,
            {
                UNUSED(model);
                instance->callback(BoilerplateCustomEventScene2Back, instance->context);
                boilerplate_play_long_bump(instance->context);
                boilerplate_led_set_rgb(instance->context, 255, 0, 0);
            },
            true);
        return true;
    }

    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        with_view_model(
            instance->view,
            BoilerplateScene2Model * model,
            {
                // Reset the game but keep best score
                int saved_best_score = model->best_score;
                strcpy(model->game_message, "New game started!");
                boilerplate_scene_2_model_init(model);
                model->best_score = saved_best_score; // Restore best score
                boilerplate_play_long_bump(instance->context);
                boilerplate_led_set_rgb(instance->context, 0, 0, 255);
            },
            true);
        return true;
    }

    if(event->type == InputTypePress) {
        switch(event->key) {
        case InputKeyUp:
            with_view_model(
                instance->view,
                BoilerplateScene2Model * model,
                {
                    // Add 1 to the player guess or reset to 0 if it's 99
                    model->player_guess = model->player_guess < 99 ? model->player_guess + 1 : 0;
                    boilerplate_play_button_press(instance->context);
                },
                true);
            break;
        case InputKeyDown:
            with_view_model(
                instance->view,
                BoilerplateScene2Model * model,
                {
                    // Subtract 1 from the player guess or set to 99 if it's 0
                    model->player_guess = model->player_guess > 0 ? model->player_guess - 1 : 99;
                    boilerplate_play_button_press(instance->context);
                },
                true);
            break;
        case InputKeyLeft:
            with_view_model(
                instance->view,
                BoilerplateScene2Model * model,
                {
                    // Subtract 10 from the player guess, if it's under 0, set it to 99
                    model->player_guess = model->player_guess > 10 ? model->player_guess - 10 : 99;
                    boilerplate_play_short_bump(instance->context);
                },
                true);
            break;
        case InputKeyRight:
            with_view_model(
                instance->view,
                BoilerplateScene2Model * model,
                {
                    // Add 10 to the player guess, if it's over 99, set it to 0
                    model->player_guess = model->player_guess < 90 ? model->player_guess + 10 : 0;
                    boilerplate_play_short_bump(instance->context);
                },
                true);
            break;
        case InputKeyOk:
            with_view_model(
                instance->view,
                BoilerplateScene2Model * model,
                {
                    if(!model->game_won) {
                        model->attempts++;

                        // Process the guess
                        if(model->target_number == model->player_guess) {
                            model->game_won = true;
                            snprintf(
                                model->game_message,
                                sizeof(model->game_message),
                                "🎉 Winner! Got it in %d tries!",
                                model->attempts);

                            // Update best score
                            if(model->best_score == 0 || model->attempts < model->best_score) {
                                model->best_score = model->attempts;
                            }

                            boilerplate_play_win_sound(instance->context);
                            boilerplate_play_happy_bump(instance->context);
                            boilerplate_led_set_rgb(instance->context, 0, 255, 0);
                            dolphin_deed(DolphinDeedPluginGameWin);
                        } else {
                            int difference = abs(model->target_number - model->player_guess);

                            if(difference <= 2) {
                                strcpy(
                                    model->game_message,
                                    model->target_number > model->player_guess ?
                                        "Very close! Go higher!" :
                                        "Very close! Go lower!");
                                boilerplate_play_close_sound(instance->context);
                                boilerplate_play_close_bump(instance->context);
                                boilerplate_led_set_rgb(
                                    instance->context, 255, 255, 0); // Yellow for close
                            } else if(difference <= 5) {
                                strcpy(
                                    model->game_message,
                                    model->target_number > model->player_guess ?
                                        "Close! Try higher!" :
                                        "Close! Try lower!");
                                boilerplate_play_close_sound(instance->context);
                                boilerplate_play_short_bump(instance->context);
                                boilerplate_led_set_rgb(
                                    instance->context, 255, 165, 0); // Orange for somewhat close
                            } else if(difference <= 15) {
                                strcpy(
                                    model->game_message,
                                    model->target_number > model->player_guess ? "Too low!" :
                                                                                 "Too high!");
                                boilerplate_play_error_sound(instance->context);
                                boilerplate_led_set_rgb(
                                    instance->context, 255, 0, 0); // Red for wrong
                            } else {
                                strcpy(
                                    model->game_message,
                                    model->target_number > model->player_guess ? "Way too low!" :
                                                                                 "Way too high!");
                                boilerplate_play_error_sound(instance->context);
                                boilerplate_led_set_rgb(
                                    instance->context, 255, 0, 0); // Red for very wrong
                            }
                        }
                        boilerplate_play_button_press(instance->context);
                    }
                },
                true);
            break;
        case InputKeyBack:
            break;
        case InputKeyMAX:
            break;
        }
    } else if(event->type == InputTypeRelease) {
        switch(event->key) {
        case InputKeyUp:
        case InputKeyDown:
        case InputKeyLeft:
        case InputKeyRight:
        case InputKeyOk:
        case InputKeyBack:
        case InputKeyMAX:
            boilerplate_play_input_sound(instance->context);
            boilerplate_led_set_rgb(instance->context, 0, 0, 0);
            boilerplate_stop_all_sound(instance->context);
            break;
        }
    }

    return true;
}

void boilerplate_scene_2_exit(void* context) {
    furi_assert(context);
    BoilerplateScene2* instance = context;
    if(instance->context) {
        boilerplate_stop_all_sound(instance->context);
        boilerplate_led_reset(instance->context);
    }
}

void boilerplate_scene_2_enter(void* context) {
    furi_assert(context);
    dolphin_deed(DolphinDeedPluginStart);
}

BoilerplateScene2* boilerplate_scene_2_alloc() {
    BoilerplateScene2* instance = malloc(sizeof(BoilerplateScene2));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(BoilerplateScene2Model));
    view_set_context(instance->view, instance);
    view_set_draw_callback(instance->view, (ViewDrawCallback)boilerplate_scene_2_draw);
    view_set_input_callback(instance->view, boilerplate_scene_2_input);
    view_set_enter_callback(instance->view, boilerplate_scene_2_enter);
    view_set_exit_callback(instance->view, boilerplate_scene_2_exit);

    with_view_model(
        instance->view,
        BoilerplateScene2Model * model,
        { boilerplate_scene_2_model_init(model); },
        true);

    return instance;
}

void boilerplate_scene_2_free(BoilerplateScene2* instance) {
    furi_assert(instance);
    view_free(instance->view);
    free(instance);
}

View* boilerplate_scene_2_get_view(BoilerplateScene2* instance) {
    furi_assert(instance);
    return instance->view;
}
