#include "playlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void createPlaylist(Playlist* pl, const char* title) {
    strncpy(pl->title, title, sizeof(pl->title));

    pl->head = NULL;
    pl->tail = NULL;
    pl->current = NULL;
    pl->count = 0;
}