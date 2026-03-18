
#ifndef PLAYLISTAPP_PLAYLIST_H
#define PLAYLISTAPP_PLAYLIST_H

#include "raylib.h"

typedef struct Song {
    char title[100];
    char artist[100];
    char album[100];
    char path[512]; // To hold the actual file path
    Music music;
} Song;

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    Song song;
} Node;

typedef struct {
    char title[100];
    Node* head;
    Node* tail;
    Node* current;
    int count;
} Playlist;

void createPlaylist(Playlist* pl, const char* title);
void addSong(Playlist* pl, Song song);
void removeSong(Playlist* pl, Node* target);
void nextSong(Playlist* pl);
void prevSong(Playlist* pl);
#endif //PLAYLISTAPP_PLAYLIST_H