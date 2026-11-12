//
//  trophy.c
//  skibidi_toilet
//
//  Created by Nahathai Wonganawat on 12/25/24.
//
//  trophy.c
//  skibidi_toilet
//
//  Created by Nahathai Wonganawat on 12/25/24.

#define _USE_MATH_DEFINES
#include "trophy.h"
#include "utility.h"
#include <math.h>
#include <stdlib.h>
#include <time.h> // For randomization

#define QUEUE_SIZE 2000
/*
    ENEMY IMPLEMENTATION
 */
ALLEGRO_BITMAP * prizeBitmap;
//static int countEscapeOptions(Map *map, Point tile);
static void updateSquarePath(Trophy *trophy, Point start);
static void teleportTrophy(Trophy *trophy, Map *map);
static int findValidPositions(Map *map, Point validPositions[], int maxPositions);
static bool isValidSquare(Map *map, Point topLeft) ;


//##########
// Return true if enemy have collision with unwalkable tiles in map
static bool isCollision(Point enemyCoord, Map* map);
// Return true if player collide with enemy
static bool playerCollision(Point enemyCoord, Point playerCoord);
void initTrophy(void){
    // For memory efficiency, we load the image once
    // as every enemy bitmap are pointer, they will point to the same global bitmap variable
    // Slime
    char * prizePath = "trophy.png";
    prizeBitmap = al_load_bitmap(prizePath);
    if(!prizeBitmap){
        game_abort("Error Load Bitmap with trophy path : %s", prizePath);
    }
}
Trophy createTrophy(int row, int col, char type){
    game_log("Creating trophy at ( %d, %d )", row, col);
    
    Trophy trophy;
    memset(&trophy, 0, sizeof(trophy));
    
    trophy.animation_tick = 0;
    trophy.death_animation_tick = 0;
    trophy.status = RUN;
    
    trophy.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };
    
    switch(type){
        case 'T':
            trophy.health = 100;
            trophy.type = prize;
            trophy.speed = 2;
            trophy.image = prizeBitmap;
            break;
        // Insert more here to have more enemy variant
        default:
            trophy.health = 100;
            trophy.type = prize;
            trophy.speed = 2;
            trophy.image = prizeBitmap;
            break;
    }
    
    return trophy;
}

bool updateTrophy(Trophy *trophy, Map *map, Player *player) {
    static int teleportCooldown = 300; // Cooldown in ticks (e.g., 300 ticks)

    if (--teleportCooldown <= 0) {
        teleportTrophy(trophy, map);
        teleportCooldown = 300; // Reset cooldown
    }

    // Continue square path movement
    updateSquarePath(trophy, trophy->coord);
    
    // Check for collision with the player
    if (playerCollision(trophy->coord, player->coord) && trophy->animation_hit_tick == 0) {
        trophy->animation_tick = 0;
        trophy->animation_hit_tick = 32;

        // Call hitPlayer_Trophy when the collision occurs
        hitPlayer_Trophy(player, trophy->coord, 10); // Adjust the damage value (10) as needed
    }

    return false;
}


void drawTrophy(Trophy *enemy, Point cam) {
    ///scale thissssss make it looks good bitch
    int dy = enemy->coord.y - cam.y; // Destination y-axis
    int dx = enemy->coord.x - cam.x; // Destination x-axis
    const int trophy_frame_width = 32;
    const int trophy_frame_height = 32;
    const int TOTAL_FRAMES = 10; // Total frames in the trophy sprite
    // Increment animation frame offset
    int offset = trophy_frame_width * (enemy->animation_tick / 8);
    if (offset >= trophy_frame_width * TOTAL_FRAMES) {
        offset = trophy_frame_width * (TOTAL_FRAMES - 1); // Stay on the last frame
    }
    // Remove green tint if not necessary
    ALLEGRO_COLOR tint_color = al_map_rgb(255, 255, 255); // Neutral color (no tint)
    if (enemy->status == RUN) {
        al_draw_tinted_scaled_rotated_bitmap_region(
            enemy->image,
            offset, 0,          // Source x and y (frame offset)
            trophy_frame_width, trophy_frame_height, // Frame size
            tint_color,         // Neutral tint
            trophy_frame_width / 2, trophy_frame_height / 2, // Rotation pivot
            dx + trophy_frame_width / 2, dy + trophy_frame_height / 2, // Destination
            1.5, 1.5,           // Scaling (no scaling in this case)
            0,                  // Rotation angle
            enemy->dir == RIGHT ? ALLEGRO_FLIP_HORIZONTAL : 0 // Flip flag
        );
    }
}
void destroyTrophy(Trophy * enemy){
}
void terminateTrophy(void) {
    al_destroy_bitmap(prizeBitmap);
}
void hitTrophy(Trophy * enemy, int damage, float angle){
    /*
        [TODO Homework]
        Decrease the enemy health with damage, if the health < 0, then set the status to DYING
    
        enemy->health = ...
        if(...){
            enemy->status = DYING;
        }
    */
    //#############################################################
    // Decrease the enemy's health with the damage
    enemy->health -= 0;// it does not change
    // If health drops below or equals zero, set the status to DYING
    if (enemy->health <= 0) {
        enemy->status = STAY;
    }
    //#############################################################
    
    // Set the knockback angle and cooldown for knockback
    enemy->knockback_angle = angle;
    enemy->knockback_CD = 0;//no knockback
}
/*
    LINKED LIST IMPLEMENTATION FOR ENEMY
    THERE ARE :
        (1) CREATE      : INITIALIZING DUMMY HEAD
        (2) INSERT      : INSERT NEW NODE OF ENEMY
        (3) UPDATE      : ITERATE EVERYTHING AND UPDATE THE ENEMY
        (4) DRAW        : ITERATE EVERYTHING AND DRAW THE ENEMY
        (5) DESTROY     : DESTROY THE LINKED LIST
 */
trophyNode * createTrophyList(void){
    trophyNode * dummyhead = (trophyNode *) malloc(sizeof(trophyNode));
    dummyhead->next = NULL;
    return dummyhead;
}
void insertTrophyList(trophyNode * dummyhead, Trophy _enemy){
    trophyNode * tmp = (trophyNode *) malloc(sizeof(trophyNode));
    tmp->enemy = _enemy;
    tmp->next = dummyhead->next;
    dummyhead->next = tmp;
}
void updateTrophyList(trophyNode * dummyhead, Map * map, Player * player){
    trophyNode * cur = dummyhead->next;
    trophyNode * prev = dummyhead;
    
    while(cur != NULL){
        bool shouldDelete = updateTrophy(&cur->enemy, map, player);
        if(shouldDelete){
            prev->next = cur->next;
            destroyTrophy(&cur->enemy);
            free(cur);
            cur = prev->next;
        }
        else{
            prev = cur;
            cur = cur->next;
        }
    }
}
void drawTrophyList(trophyNode * dummyhead, Point cam){
    trophyNode * cur = dummyhead->next;
    
    while(cur != NULL){
        drawTrophy(&cur->enemy, cam);
        cur = cur->next;
    }
}
void destroyTrophyList(trophyNode * dummyhead){
    while(dummyhead != NULL){
        trophyNode * del = dummyhead;
        dummyhead = dummyhead->next;
        destroyTrophy(&del->enemy);
        free(del);
    }
}
//#########################################

static void updateSquarePath(Trophy *trophy, Point start) {
    // Define the corners of the 4x4 square
    static Point squareCorners[4];
    if (trophy->squarePathStep == 0) {
        squareCorners[0] = start;                                    // Top-left
        squareCorners[1] = (Point){start.x + 3 * TILE_SIZE, start.y}; // Top-right
        squareCorners[2] = (Point){start.x + 3 * TILE_SIZE, start.y + 3 * TILE_SIZE}; // Bottom-right
        squareCorners[3] = (Point){start.x, start.y + 3 * TILE_SIZE}; // Bottom-left
    }

    // Get the current target point
    Point target = squareCorners[trophy->squarePathStep];

    // Calculate direction to move toward the target
    Point delta = {
        (target.x > trophy->coord.x) ? trophy->speed : (target.x < trophy->coord.x) ? -trophy->speed : 0,
        (target.y > trophy->coord.y) ? trophy->speed : (target.y < trophy->coord.y) ? -trophy->speed : 0
    };

    // Update trophy position
    trophy->coord.x += delta.x;
    trophy->coord.y += delta.y;

    // Check if the trophy has reached the target
    if (trophy->coord.x == target.x && trophy->coord.y == target.y) {
        // Move to the next corner
        trophy->squarePathStep = (trophy->squarePathStep + 1) % 4;
    }
}
static bool isValidSquare(Map *map, Point topLeft) {
    for (int dx = 0; dx < 4; dx++) {
        for (int dy = 0; dy < 4; dy++) {
            int x = topLeft.x + dx;
            int y = topLeft.y + dy;

            // Check bounds and walkability
            if (x < 0 || y < 0 || x >= map->col || y >= map->row || !isWalkable(map->map[y][x])) {
                return false;
            }
        }
    }
    return true;
}
static int findValidPositions(Map *map, Point validPositions[], int maxPositions) {
    int count = 0;

    for (int y = 0; y < map->row; y++) {
        for (int x = 0; x < map->col; x++) {
            Point topLeft = {x, y};
            if (isValidSquare(map, topLeft)) {
                if (count < maxPositions) {
                    validPositions[count++] = topLeft;
                }
            }
        }
    }

    return count; // Return the number of valid positions
}



static void teleportTrophy(Trophy *trophy, Map *map) {
    Point validPositions[1000]; // Arbitrary max number of valid positions
    int validCount = findValidPositions(map, validPositions, 1000);

    if (validCount > 0) {
        // Randomly choose a valid position
        srand((unsigned int)time(NULL)); // Cast time_t to unsigned int
        int randomIndex = rand() % validCount;

        // Teleport the trophy
        trophy->coord = (Point){
            validPositions[randomIndex].x * TILE_SIZE,
            validPositions[randomIndex].y * TILE_SIZE
        };

        // Reset the square path step to start a new 4x4 square path
        trophy->squarePathStep = 0;
    }
}



//#################
static bool playerCollision(Point enemyCoord, Point playerCoord){
    // Rectangle & Rectanlge Collision
    if (enemyCoord.x < playerCoord.x + TILE_SIZE &&
        enemyCoord.x + TILE_SIZE > playerCoord.x &&
        enemyCoord.y < playerCoord.y + TILE_SIZE &&
        enemyCoord.y + TILE_SIZE > playerCoord.y) {
            return true;
    } else {
        return false;
    }
}
    
static bool isCollision(Point enemyCoord, Map* map){
    if( enemyCoord.x < 0 ||
        enemyCoord.y < 0 ||
        (enemyCoord.x + TILE_SIZE - 1) / TILE_SIZE >= map->col ||
        (enemyCoord.y + TILE_SIZE - 1) / TILE_SIZE >= map->row)
        return true;
    /*
        [TODO HACKATHON 2-2]
        Check every corner of enemy if it's collide or not
        Return true if it's not walkable
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
    */
    //######################################################################
    int left = enemyCoord.x / TILE_SIZE;
    int right = (enemyCoord.x + TILE_SIZE - 1) / TILE_SIZE;
    int top = enemyCoord.y / TILE_SIZE;
    int bottom = (enemyCoord.y + TILE_SIZE - 1) / TILE_SIZE;
    if (!isWalkable(map->map[top][left])) return true;    // Top-left
    if (!isWalkable(map->map[top][right])) return true;   // Top-right
    if (!isWalkable(map->map[bottom][left])) return true; // Bottom-left
    if (!isWalkable(map->map[bottom][right])) return true; // Bottom-right
    //######################################################################
    return false;
}
