#include "ui_components.h"
#include "state.h"
#include <string.h>

static bool ExtEqualsIgnoreCase(const char* left, const char* right) {
    if (!left || !right) return false;

    while (*left && *right) {
        char l = *left;
        char r = *right;

        if (l >= 'A' && l <= 'Z') l = (char)(l + ('a' - 'A'));
        if (r >= 'A' && r <= 'Z') r = (char)(r + ('a' - 'A'));
        if (l != r) return false;

        left++;
        right++;
    }

    return *left == '\0' && *right == '\0';
}

static bool IsCtrlDown(void) {
    return IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
}

static void AppendSanitizedText(char* buffer, int* count, int max, const char* text) {
    if (!buffer || !count || !text || max <= 1) return;

    while (*text && *count < max - 1) {
        unsigned char c = (unsigned char)*text++;


        if (c == '\r' || c == '\n' || c == '\t') c = ' ';

        if (c >= 32 && c <= 125) {
            buffer[*count] = (char)c;
            (*count)++;
        }
    }

    buffer[*count] = '\0';
}

bool IconButton(Rectangle bounds, const char* text, Color color, bool filled) {
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, bounds);
    if (filled) DrawRectangleRounded(bounds, 0.2, 8, hover ? Fade(color, 0.8f) : color);
    else DrawRectangleRounded(bounds, 0.2, 8, hover ? COLOR_HOVER : BLANK);
    DrawText(text, bounds.x + (bounds.width/2 - MeasureText(text, 16)/2), bounds.y + (bounds.height/2 - 8), 16, (filled && !hover) ? BLACK : WHITE);
    return hover && IsMouseButtonPressed(0);
}

void HandleTextInput(char* buffer, int* count, int max) {
    if (!buffer || !count || max <= 1) return;

    bool ctrlDown = IsCtrlDown();

    if (ctrlDown) {
        if (IsKeyPressed(KEY_C)) {
            SetClipboardText(buffer);
        }
        if (IsKeyPressed(KEY_X)) {
            SetClipboardText(buffer);
            buffer[0] = '\0';
            *count = 0;
        }
        if (IsKeyPressed(KEY_V)) {
            AppendSanitizedText(buffer, count, max, GetClipboardText());
        }
    }

    int key = GetCharPressed();
    while (key > 0) {
        if (!ctrlDown && (key >= 32) && (key <= 125) && (*count < max - 1)) {
            buffer[*count] = (char)key; buffer[*count + 1] = '\0'; (*count)++;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && *count > 0) { (*count)--; buffer[*count] = '\0'; }
}

bool IsValidAudioFile(const char* ext) {
    if (ext == NULL) return false;
    return ExtEqualsIgnoreCase(ext, ".mp3") || ExtEqualsIgnoreCase(ext, ".wav");
}
