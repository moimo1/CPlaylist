
#ifndef PLAYLISTAPP_PLAYLIST_H
#define PLAYLISTAPP_PLAYLIST_H

#include "raylib.h"

typedef struct Song {
    char title[100];
    char artist[100];
    char album[100];
    Music music;
} Song;

typedef struct Node {
    Song song;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    Node* current;
    int count;
} Playlist;
#endif //PLAYLISTAPP_PLAYLIST_H