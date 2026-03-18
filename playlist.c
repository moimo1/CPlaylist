#include "playlist.h"
#include <stdlib.h>
#include <string.h>

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

void removeSong(Playlist* pl, Node* target) {
    if (!pl || !target) return;

    if (pl->current == target) {
        pl->current = target->next ? target->next : target->prev;
    }

    if (target->next) target->next->prev = target->prev;
    if (target->prev) target->prev->next = target->next;

    if (target == pl->head) pl->head = target->next;
    if (target == pl->tail) pl->tail = target->prev;

    // Optional: Only unload if it's actually loaded
    if (target->song.music.stream.buffer != NULL) {
        UnloadMusicStream(target->song.music);
    }
    
    free(target);
    pl->count--;
}

void nextSong(Playlist* pl) {
    if (pl->current != NULL && pl->current->next != NULL) {
        pl->current = pl->current->next;
    } else if (pl->head != NULL) {
        pl->current = pl->head;
    }
}

void prevSong(Playlist* pl) {
    if (pl->current != NULL && pl->current->prev != NULL) {
        pl->current = pl->current->prev;
    } else if (pl->tail != NULL) {
        pl->current = pl->tail;
    }
}