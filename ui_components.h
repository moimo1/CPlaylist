#ifndef PLAYLISTAPP_UI_COMPONENTS_H
#define PLAYLISTAPP_UI_COMPONENTS_H

#include "raylib.h"
#include <stdbool.h>

bool IconButton(Rectangle bounds, const char* text, Color color, bool filled);
void HandleTextInput(char* buffer, int* count, int max);
bool IsValidAudioFile(const char* ext);

#endif //PLAYLISTAPP_UI_COMPONENTS_H
