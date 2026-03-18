#include "raylib.h"
#include "playlist.h"
#include "state.h"
#include "screens.h"
#include "ui_components.h"
#include <string.h>
#include <stdio.h>

void SavePlaylists(AppState* state) {
    FILE* file = fopen("playlists.dat", "wb");
    if (!file) return;

    fwrite(&state->count, sizeof(int), 1, file);
    for (int i = 0; i < state->count; i++) {
        Playlist* pl = &state->playlists[i];
        fwrite(pl->title, sizeof(char), 100, file);
        fwrite(&pl->count, sizeof(int), 1, file);

        Node* current = pl->head;
        while (current) {
            Song songData = current->song;
            fwrite(songData.title, sizeof(char), 100, file);
            fwrite(songData.artist, sizeof(char), 100, file);
            fwrite(songData.album, sizeof(char), 100, file);
            fwrite(songData.path, sizeof(char), 512, file);
            current = current->next;
        }
    }
    fclose(file);
}

void LoadPlaylists(AppState* state) {
    FILE* file = fopen("playlists.dat", "rb");
    if (!file) return;

    int playlistCount = 0;
    if (fread(&playlistCount, sizeof(int), 1, file) != 1) {
        fclose(file);
        return;
    }

    state->count = playlistCount;
    for (int i = 0; i < playlistCount; i++) {
        Playlist* pl = &state->playlists[i];
        fread(pl->title, sizeof(char), 100, file);
        
        pl->head = NULL;
        pl->tail = NULL;
        pl->current = NULL;
        pl->count = 0;

        int songCount = 0;
        if (fread(&songCount, sizeof(int), 1, file) != 1) continue;

        for (int j = 0; j < songCount; j++) {
            Song song;
            fread(song.title, sizeof(char), 100, file);
            fread(song.artist, sizeof(char), 100, file);
            fread(song.album, sizeof(char), 100, file);
            fread(song.path, sizeof(char), 512, file);
            
            song.music = LoadMusicStream(song.path);
            addSong(pl, song);
        }
    }
    fclose(file);
}

int main() {
    InitWindow(1200, 850, "Spotify Clone - Raylib Edition");
    InitAudioDevice();
    SetTargetFPS(60);
    AppState state = { .count = 0, .selectedIdx = -1, .currentScreen = SCREEN_HOME, .activeField = 0, .activeModal = MODAL_NONE };
    LoadPlaylists(&state);

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
                    state.songPathError = false;
                } else {
                    state.songPathError = true;
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

    SavePlaylists(&state);

    // Cleanup Playlists
    for (int i = 0; i < state.count; i++) {
        deletePlaylistAt(&state, 0); 
    }
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}