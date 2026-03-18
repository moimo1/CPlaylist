#include "raylib.h"
#include "playlist.h"
#include "state.h"
#include "screens.h"
#include "ui_components.h"
#include <string.h>

int main() {
    InitWindow(1200, 850, "Spotify Clone - Raylib Edition");
    InitAudioDevice();
    SetTargetFPS(60);
    AppState state = { .count = 0, .selectedIdx = -1, .currentScreen = SCREEN_HOME, .activeField = 0 };

    while (!WindowShouldClose()) {
        // --- LOGIC UPDATES ---
        if (state.selectedIdx != -1 && state.playlists[state.selectedIdx].current) {
            UpdateMusicStream(state.playlists[state.selectedIdx].current->song.music);
        }

        // --- HANDLE DRAG & DROP ---
        if (state.activeModal == MODAL_SONG && IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();
            if (droppedFiles.count > 0) {
                const char* ext = GetFileExtension(droppedFiles.paths[0]);
                if (IsValidAudioFile(ext)) {
                    strncpy(state.buffer3, droppedFiles.paths[0], 255);
                    state.buffer3[255] = '\0';
                    state.letterCount3 = strlen(state.buffer3);
                }
            }
            UnloadDroppedFiles(droppedFiles);
        } else if (IsFileDropped()) {
            FilePathList dropped = LoadDroppedFiles();
            UnloadDroppedFiles(dropped);
        }

        BeginDrawing();
        ClearBackground(COLOR_BG);

        int mainX = 270;
        int mainW = GetScreenWidth() - 300;

        DrawSidebar(&state, mainX, mainW);

        if (state.currentScreen == SCREEN_HOME) {
            DrawHomeScreen(&state, mainX, mainW);
        } else if (state.selectedIdx != -1) {
            DrawPlaylistScreen(&state, mainX, mainW);
        }

        DrawPlayerBar(&state);
        DrawModals(&state);

        EndDrawing();
    }

    // Cleanup Playlists
    for (int i = 0; i < state.count; i++) {
        // Since we extracted destroyPlaylist into screens.c, 
        // we can just repeatedly delete the 0th item as a hack or manually free.
        // It's cleaner if we exported destroyPlaylist, but for simplicity here over deletion loop:
        deletePlaylistAt(&state, 0); 
    }
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}