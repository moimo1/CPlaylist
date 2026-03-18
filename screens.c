#include "screens.h"
#include "ui_components.h"
#include <string.h>
#include <stdlib.h>

void destroyPlaylist(Playlist* pl) {
    Node* curr = pl->head;
    while (curr) {
        Node* next = curr->next;
        free(curr);
        curr = next;
    }
    pl->head = pl->tail = pl->current = NULL;
    pl->count = 0;
}

void deletePlaylistAt(AppState* state, int index) {
    destroyPlaylist(&state->playlists[index]);
    for (int i = index; i < state->count - 1; i++) state->playlists[i] = state->playlists[i + 1];
    state->count--;
    if (state->selectedIdx == index) {
        state->selectedIdx = -1;
        state->currentScreen = SCREEN_HOME;
    } else if (state->selectedIdx > index) state->selectedIdx--;
}

void DrawSidebar(AppState* state, int mainX, int mainW) {
    DrawRectangle(0, 0, 240, GetScreenHeight() - 100, COLOR_SIDEBAR);
    if (IconButton((Rectangle){15, 30, 210, 45}, "Home", state->currentScreen == SCREEN_HOME ? COLOR_ACCENT : WHITE, false)) {
        state->currentScreen = SCREEN_HOME;
    }

    DrawText("YOUR LIBRARY", 25, 100, 12, COLOR_TEXT_DIM);
    if (IconButton((Rectangle){200, 95, 25, 25}, "+", COLOR_ACCENT, false)) {
        state->activeModal = MODAL_PLAYLIST; state->isEditing = false; state->buffer1[0] = '\0'; state->letterCount1 = 0;
    }

    for (int i = 0; i < state->count; i++) {
        Rectangle row = {10, 130 + (i * 35), 220, 30};
        bool hover = CheckCollisionPointRec(GetMousePosition(), row);
        if (hover) DrawRectangleRounded(row, 0.2, 8, COLOR_HOVER);

        Color textCol = (state->currentScreen == SCREEN_PLAYLIST_DETAIL && state->selectedIdx == i) ? COLOR_ACCENT : WHITE;
        DrawText(state->playlists[i].title, 25, row.y + 8, 15, textCol);

        if (hover && IsMouseButtonPressed(0)) {
            state->selectedIdx = i; state->currentScreen = SCREEN_PLAYLIST_DETAIL;
        }
    }
}

void DrawHomeScreen(AppState* state, int mainX, int mainW) {
    DrawText("Good Afternoon", mainX, 40, 32, WHITE);

    int cardW = 180, cardH = 240, spacing = 20, cols = mainW / (cardW + spacing);

    for (int i = 0; i < state->count; i++) {
        int row = i / cols;
        int col = i % cols;
        Rectangle card = { mainX + col * (cardW + spacing), 100 + row * (cardH + spacing), cardW, cardH };

        bool hover = CheckCollisionPointRec(GetMousePosition(), card);
        DrawRectangleRounded(card, 0.05, 8, hover ? COLOR_HOVER : COLOR_CARD);

        Rectangle cover = { card.x + 15, card.y + 15, cardW - 30, cardW - 30 };
        DrawRectangleGradientV(cover.x, cover.y, cover.width, cover.height, COLOR_HOVER, COLOR_BG);
        DrawText("♫", cover.x + cover.width/2 - 10, cover.y + cover.height/2 - 20, 40, COLOR_TEXT_DIM);

        DrawText(state->playlists[i].title, card.x + 15, card.y + 175, 18, WHITE);
        DrawText(TextFormat("%d songs", state->playlists[i].count), card.x + 15, card.y + 200, 14, COLOR_TEXT_DIM);

        if (hover) {
            if (IconButton((Rectangle){card.x + card.width - 35, card.y + 5, 30, 30}, "X", COLOR_DANGER, true)) {
                deletePlaylistAt(state, i);
                break;
            }
            if (IsMouseButtonPressed(0) && GetMouseY() > card.y + 40) {
                state->selectedIdx = i;
                state->currentScreen = SCREEN_PLAYLIST_DETAIL;
            }
        }
    }
}

void DrawPlaylistScreen(AppState* state, int mainX, int mainW) {
    Playlist* active = &state->playlists[state->selectedIdx];

    DrawRectangleGradientV(240, 0, GetScreenWidth()-240, 300, COLOR_HOVER, COLOR_BG);
    DrawText(active->title, mainX, 120, 72, WHITE);

    if (IconButton((Rectangle){mainX, 220, 100, 35}, "Rename", COLOR_HOVER, true)) {
        state->activeModal = MODAL_PLAYLIST; state->isEditing = true;
        strcpy(state->buffer1, active->title); state->letterCount1 = strlen(state->buffer1);
    }
    if (IconButton((Rectangle){mainX + 110, 220, 100, 35}, "Delete", COLOR_DANGER, true)) {
        deletePlaylistAt(state, state->selectedIdx);
        return;
    }
    if (IconButton((Rectangle){mainX + 220, 220, 130, 35}, "+ Add Song", COLOR_ACCENT, true)) {
        state->activeModal = MODAL_SONG; state->isEditing = false;
        state->buffer1[0] = state->buffer2[0] = state->buffer3[0] = '\0';
        state->letterCount1 = state->letterCount2 = state->letterCount3 = state->activeField = 0;
    }

    int tableY = 320;
    DrawText("#  TITLE", mainX, tableY, 14, COLOR_TEXT_DIM);
    DrawText("ARTIST", mainX + 300, tableY, 14, COLOR_TEXT_DIM);
    DrawLine(mainX, tableY + 25, mainX + mainW, tableY + 25, COLOR_HOVER);

    Node* curr = active->head;
    for (int i = 0; curr != NULL; i++, curr = curr->next) {
        Rectangle row = {mainX - 10, tableY + 35 + (i*45), mainW + 20, 40};
        bool hover = CheckCollisionPointRec(GetMousePosition(), row);
        if (hover) DrawRectangleRounded(row, 0.2, 8, COLOR_HOVER);

        Color tCol = (active->current == curr) ? COLOR_ACCENT : WHITE;
        DrawText(TextFormat("%d", i+1), mainX, row.y + 12, 16, tCol);
        DrawText(curr->song.title, mainX + 40, row.y + 12, 16, tCol);
        DrawText(curr->song.artist, mainX + 300, row.y + 12, 16, COLOR_TEXT_DIM);

        if (hover) {
            if (IconButton((Rectangle){row.x + row.width - 80, row.y + 8, 30, 25}, "E", COLOR_ACCENT, false)) {
                state->activeModal = MODAL_SONG; state->isEditing = true; state->editingSong = curr;
                strcpy(state->buffer1, curr->song.title); state->letterCount1 = strlen(state->buffer1);
                strcpy(state->buffer2, curr->song.artist); state->letterCount2 = strlen(state->buffer2);
                strcpy(state->buffer3, curr->song.path); state->letterCount3 = strlen(state->buffer3);
            }
            if (IconButton((Rectangle){row.x + row.width - 40, row.y + 8, 30, 25}, "X", COLOR_DANGER, false)) {
                removeSong(active, curr); break;
            }
        }
        if (hover && IsMouseButtonPressed(0) && GetMouseX() < row.x + row.width - 80) {
            active->current = curr;
            if (curr->song.music.stream.buffer != NULL) PlayMusicStream(curr->song.music);
        }
    }
}

void DrawPlayerBar(AppState* state) {
    DrawRectangle(0, GetScreenHeight() - 100, GetScreenWidth(), 100, COLOR_CARD);
    DrawLine(0, GetScreenHeight() - 100, GetScreenWidth(), GetScreenHeight() - 100, COLOR_HOVER);

    if (state->selectedIdx != -1 && state->playlists[state->selectedIdx].current) {
        Song* s = &state->playlists[state->selectedIdx].current->song;
        DrawText(s->title, 25, GetScreenHeight() - 70, 18, WHITE);
        DrawText(s->artist, 25, GetScreenHeight() - 45, 14, COLOR_TEXT_DIM);

        Rectangle bPrev = {GetScreenWidth()/2 - 100, GetScreenHeight() - 70, 40, 40};
        Rectangle bPlay = {GetScreenWidth()/2 - 40,  GetScreenHeight() - 75, 50, 50};
        Rectangle bNext = {GetScreenWidth()/2 + 30,  GetScreenHeight() - 70, 40, 40};

        if (IconButton(bPrev, "|<", COLOR_HOVER, true)) {
            if (IsMusicStreamPlaying(s->music)) StopMusicStream(s->music);
            prevSong(&state->playlists[state->selectedIdx]);
            Song* ns = &state->playlists[state->selectedIdx].current->song;
            if (ns->music.stream.buffer != NULL) PlayMusicStream(ns->music);
        }

        bool isPlaying = IsMusicStreamPlaying(s->music);
        if (IconButton(bPlay, isPlaying ? "||" : "  >  ", COLOR_ACCENT, true)) {
            if (isPlaying) PauseMusicStream(s->music);
            else ResumeMusicStream(s->music);
        }

        if (IconButton(bNext, ">|", COLOR_HOVER, true)) {
            if (IsMusicStreamPlaying(s->music)) StopMusicStream(s->music);
            nextSong(&state->playlists[state->selectedIdx]);
            Song* ns = &state->playlists[state->selectedIdx].current->song;
            if (ns->music.stream.buffer != NULL) PlayMusicStream(ns->music);
        }
    }
}

void DrawModals(AppState* state) {
    if (state->activeModal == MODAL_NONE) return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 200});
    Rectangle mRec = { GetScreenWidth()/2 - 200, GetScreenHeight()/2 - 150, 400, 300 };
    DrawRectangleRounded(mRec, 0.1, 8, COLOR_CARD);

    if (state->activeModal == MODAL_PLAYLIST) {
        DrawText(state->isEditing ? "Rename Playlist" : "New Playlist", mRec.x + 25, mRec.y + 25, 22, WHITE);
        DrawRectangleRounded((Rectangle){mRec.x + 25, mRec.y + 100, 350, 50}, 0.2, 8, COLOR_BG);
        DrawText(state->buffer1, mRec.x + 40, mRec.y + 115, 18, WHITE);
        HandleTextInput(state->buffer1, &state->letterCount1, 64);

        if (IconButton((Rectangle){mRec.x + 210, mRec.y + 220, 165, 50}, "Save", COLOR_ACCENT, true)) {
            if (state->isEditing) strncpy(state->playlists[state->selectedIdx].title, state->buffer1, 99);
            else if (state->count < MAX_PLAYLISTS) createPlaylist(&state->playlists[state->count++], state->buffer1);
            state->activeModal = MODAL_NONE;
        }
    }
    else if (state->activeModal == MODAL_SONG) {
        char* bufs[] = {state->buffer1, state->buffer2, state->buffer3}; 
        int* counts[] = {&state->letterCount1, &state->letterCount2, &state->letterCount3};
        const char* labels[] = {"Title", "Artist", "File Path (Drag & Drop .mp3 / .wav)"};

        DrawText(state->isEditing ? "Edit Song" : "Add Song", mRec.x + 25, mRec.y + 25, 22, WHITE);
        for(int i=0; i<3; i++) {
            DrawText(labels[i], mRec.x+25, mRec.y + 60 + (i*60), 12, COLOR_TEXT_DIM);
            Rectangle f = {mRec.x+25, mRec.y + 75 + (i*60), 350, 35};
            DrawRectangleRounded(f, 0.2, 8, state->activeField == i ? COLOR_HOVER : COLOR_BG);
            
            if (i == 2 && *counts[2] > 38) {
                DrawText(TextFormat("...%s", bufs[i] + (*counts[2] - 35)), f.x+10, f.y+10, 14, WHITE);
            } else {
                DrawText(bufs[i], f.x+10, f.y+10, 14, WHITE);
            }

            if (CheckCollisionPointRec(GetMousePosition(), f) && IsMouseButtonPressed(0)) state->activeField = i;
        }
        
        HandleTextInput(bufs[state->activeField], counts[state->activeField], state->activeField == 2 ? 255 : 64);
        
        if (IconButton((Rectangle){mRec.x+210, mRec.y+250, 165, 40}, "Done", COLOR_ACCENT, true)) {
            if (state->isEditing) {
                strcpy(state->editingSong->song.title, state->buffer1); 
                strcpy(state->editingSong->song.artist, state->buffer2);
                if (strcmp(state->editingSong->song.path, state->buffer3) != 0) {
                    strcpy(state->editingSong->song.path, state->buffer3);
                    if (state->editingSong->song.music.stream.buffer != NULL) UnloadMusicStream(state->editingSong->song.music);
                    state->editingSong->song.music = LoadMusicStream(state->buffer3);
                }
            } else {
                Song s = {0}; 
                strcpy(s.title, strlen(state->buffer1) > 0 ? state->buffer1 : "Unknown Title"); 
                strcpy(s.artist, strlen(state->buffer2) > 0 ? state->buffer2 : "Unknown Artist");
                strcpy(s.path, state->buffer3);
                if (strlen(state->buffer3) > 0) s.music = LoadMusicStream(state->buffer3);
                
                addSong(&state->playlists[state->selectedIdx], s);
            }
            state->activeModal = MODAL_NONE;
        }
        if (IconButton((Rectangle){mRec.x + 25, mRec.y + 250, 165, 40}, "Cancel", COLOR_HOVER, true)) state->activeModal = MODAL_NONE;
    }
    if (state->activeModal == MODAL_PLAYLIST && IconButton((Rectangle){mRec.x + 25, mRec.y + mRec.height - 70, 165, 45}, "Cancel", COLOR_HOVER, true)) state->activeModal = MODAL_NONE;
}
