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

void addSong(Playlist* pl, Song song) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->song = song;
    newNode->next = NULL;
    newNode->prev = pl->tail;

    if(pl->tail == NULL) {
        pl->head = newNode;
        pl->current = newNode;
    } else {
        pl->tail->next = newNode;
    }

    pl->tail = newNode;
    pl->count++;
}