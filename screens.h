#ifndef PLAYLISTAPP_SCREENS_H
#define PLAYLISTAPP_SCREENS_H

#include "state.h"

void DrawSidebar(AppState* state, int mainX, int mainW);
void DrawHomeScreen(AppState* state, int mainX, int mainW);
void DrawPlaylistScreen(AppState* state, int mainX, int mainW);
void DrawPlayerBar(AppState* state);
void DrawModals(AppState* state);

void deletePlaylistAt(AppState* state, int index);

#endif //PLAYLISTAPP_SCREENS_H
