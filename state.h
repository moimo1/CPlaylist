#ifndef PLAYLISTAPP_STATE_H
#define PLAYLISTAPP_STATE_H

#include "raylib.h"
#include "playlist.h"

// --- UI THEME ---
#define MAX_PLAYLISTS 15
#define COLOR_BG      (Color){ 12, 12, 12, 255 }
#define COLOR_SIDEBAR (Color){ 0, 0, 0, 255 }
#define COLOR_CARD    (Color){ 24, 24, 24, 255 }
#define COLOR_HOVER   (Color){ 40, 40, 40, 255 }
#define COLOR_ACCENT  (Color){ 30, 215, 96, 255 }
#define COLOR_DANGER  (Color){ 220, 53, 69, 255 }
#define COLOR_TEXT_DIM (Color){ 170, 170, 170, 255 }

typedef enum { SCREEN_HOME, SCREEN_PLAYLIST_DETAIL } Screen;
typedef enum { MODAL_NONE, MODAL_PLAYLIST, MODAL_SONG } ModalType;

typedef struct {
    Playlist playlists[MAX_PLAYLISTS];
    int count;
    int selectedIdx;
    Screen currentScreen;
    ModalType activeModal;
    bool isEditing;
    Node* editingSong;

    // Input
    char buffer1[64], buffer2[64], buffer3[256];
    int letterCount1, letterCount2, letterCount3, activeField;
    bool songPathError;
} AppState;

#endif //PLAYLISTAPP_STATE_H
