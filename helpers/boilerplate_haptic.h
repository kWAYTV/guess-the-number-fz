#include <notification/notification_messages.h>

// Haptic feedback for different game events
void boilerplate_play_happy_bump(void* context); // Victory celebration
void boilerplate_play_bad_bump(void* context); // Error/wrong guess

void boilerplate_play_short_bump(void* context); // Quick feedback
void boilerplate_play_long_bump(void* context); // Reset/major action
void boilerplate_play_close_bump(void* context); // Very close guess

void boilerplate_play_button_press(void* context); // Button press feedback
