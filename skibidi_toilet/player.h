#ifndef player_h
#define player_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "utility.h"
#include "map.h"

typedef enum {
    PLAYER_IDLE, PLAYER_WALKING, PLAYER_DYING, PLAYER_WIN, PLAYER_LOSE
} PLAYER_STATUS;

typedef struct _Player{
    Point coord; // coordinate of the player
    int speed; // TODO: CHANGE SPEED SCALED TO THE SIZE OF THE TILES
    int direction;
    int health;
//    int max_health;// PLENG ADD TO UPDATE PLAYER HEALTH ON SHOP
    int money; // PLENG ADD it to save money
    int win;
    int currentGun;//PLENG ADD to choose gun NO NEED
    ALLEGRO_BITMAP* image;
    uint8_t animation_tick; // For animation
    
    float knockback_angle;
    uint8_t knockback_CD;

    PLAYER_STATUS status;

} Player;
extern Player player; //add this so we can access


Player create_player(char * path,int row,int col);
void update_player(Player * player, Map * map);
void draw_player(Player * player, Point cam);
void delete_player(Player * player);
void hitPlayer(Player * player, Point enemy_coord, int damage);
void hitPlayer_Trophy(Player * player, Point enemy_coord, int damage);//PLENG ADD this function is to hit trophy then win

#endif /* player_h */
