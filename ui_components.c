#include "ui_components.h"
#include "state.h"
#include <string.h>

bool IconButton(Rectangle bounds, const char* text, Color color, bool filled) {
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, bounds);
    if (filled) DrawRectangleRounded(bounds, 0.2, 8, hover ? Fade(color, 0.8f) : color);
    else DrawRectangleRounded(bounds, 0.2, 8, hover ? COLOR_HOVER : BLANK);
    DrawText(text, bounds.x + (bounds.width/2 - MeasureText(text, 16)/2), bounds.y + (bounds.height/2 - 8), 16, (filled && !hover) ? BLACK : WHITE);
    return hover && IsMouseButtonPressed(0);
}

void HandleTextInput(char* buffer, int* count, int max) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (*count < max - 1)) {
            buffer[*count] = (char)key; buffer[*count + 1] = '\0'; (*count)++;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && *count > 0) { (*count)--; buffer[*count] = '\0'; }
}

bool IsValidAudioFile(const char* ext) {
    if (ext == NULL) return false;
    return (strcmp(ext, ".mp3") == 0 || strcmp(ext, ".wav") == 0 || strcmp(ext, ".ogg") == 0 || strcmp(ext, ".flac") == 0);
}
