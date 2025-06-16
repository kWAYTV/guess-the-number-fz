#define NOTE_INPUT 587.33f

// Audio feedback for different game events  
void boilerplate_play_input_sound(void* context);   // General button sounds
void boilerplate_play_win_sound(void* context);     // Victory melody
void boilerplate_play_close_sound(void* context);   // Close guess feedback
void boilerplate_play_error_sound(void* context);   // Wrong guess feedback
void boilerplate_stop_all_sound(void* context);     // Stop all audio
