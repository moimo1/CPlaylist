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

void removeSong(Playlist* pl, Song song) {
    Node* temp = pl->head;

    while (temp != NULL) {
        if (strcmp(temp->song.title, song.title) == 0) {
            if (pl->current == temp) {
                pl->current = temp->next ? temp->next : temp->prev;
            }

            if (temp->next) {
                temp->next->prev = temp->prev;
            }
            if (temp->prev) {
                temp->prev->next = temp->next;
            }

            if (temp == pl->head) {
                pl->head = temp->next;
            }
            if (temp == pl->tail) {
                pl->tail = temp->prev;
            }

            free(temp);
            pl->count--;
            return;
        }
        temp = temp->next;
    }
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