//
//  trophy.h
//  skibidi_toilet
//
//  Created by Nahathai Wonganawat on 12/25/24.
//

#ifndef trophy_h
#define trophy_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "trophy.h"
#include "utility.h"
#include "map.h"
#include "player.h"
#include "UI.h"


typedef enum{
    prize,
} trophyType;

typedef enum{
    RUN, STAY, STOP
} TrophyStatus;

typedef struct _trophy{
    ALLEGRO_BITMAP* image;
    
    int squarePathStep; // Add this for the square path PLENG

    trophyType type;
    Point coord;
    int speed;
    int health;
    
    TrophyStatus status;
    
    // Knockback
    float knockback_angle;
    uint8_t knockback_CD;
    
    // Animation
    DIRECTION dir;
    uint8_t animation_tick;
    uint8_t animation_hit_tick;
    uint8_t death_animation_tick;

} Trophy;

// Create an enemy in specified row col and the type
Trophy createTrophy(int row, int col, char type);

// Return true if the enemy is death by hit
void hitTrophy(Trophy * trophy, int damage, float angle);


/*
    Struct of 1 node of enemy
    We use linked list data structure to store enemy information
    Since the enemy might die at one point, linked list will be better here since the deletion is O(1)
*/
typedef struct _trophyNode {
    Trophy enemy;
    struct _trophyNode * next;
} trophyNode;

//
//Point backtrackPath(Point src, Point dst, DIRECTION dir[MAX_MAP_ROW][MAX_MAP_COL]);//PLENG
//int evaluateSafety(Map *map, Point tile, Point player, trophyNode *enemyList);// PLENG

trophyNode * createTrophyList(void);
void insertTrophyList(trophyNode * dummyhead, Trophy _enemy);
void updateTrophyList(trophyNode * dummyhead, Map * map, Player * player);
void drawTrophyList(trophyNode * dummyhead, Point cam);
void destroyTrophyList(trophyNode * dummyhead);

void initTrophy(void);
void terminateTrophy(void);




#endif
