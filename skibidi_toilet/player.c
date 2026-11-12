#include "player.h"
#include "game.h"
#include "weapon.h"//PLENG ADD FOR WEAPON
#include "utility.h"
#include "map.h"

#include <math.h>

//#########################################################
#define FRAME_WIDTH 32
#define FRAME_HEIGHT 32
#define IDLE_FRAMES 2
#define WALKING_FRAMES 4
#define DYING_FRAMES 4
#define FRAME_DELAY 10 // Adjust this value to control the animation speed

//#########################################################

static bool isCollision(Player* player, Map* map);

Player create_player(char * path, int row, int col){
    Player player;
    memset(&player, 0, sizeof(player));
    
    player.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };
    


    player.speed = get_player_speed(); // Use saved or default speed
    player.health = get_player_health();/*(saved_health > 0) ? saved_health : base_health;*/ // Use saved or default health
    player.money = 0; // Initial money value

    
    player.image = al_load_bitmap(path);
    if(!player.image){
        game_abort("Error Load Bitmap with path : %s", path);
    }
    
    return player;
}

void update_player(Player * player, Map* map){

    Point original = player->coord;
    
    // Knockback effect
    
    if(player->knockback_CD > 0){
//        game_log("knockk knokck");//checl
        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle) *1.5;//i added * 1.5
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle) *1.5;// "" to make it go further
        Point next;
        next = (Point){next_x, player->coord.y};
        
        if(!isCollision(player, map)){
            player->coord = next;
        }

        next = (Point){player->coord.x, next_y};
        if(!isCollision(player, map)){
            player->coord = next;
        }
    }



    /*
        [TODO HACKATHON 1-1] 
        
        Player Movement
        Adjust the movement by player->speed

        if (keyState[ALLEGRO_KEY_W]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_S]) {
            player->coord.y = ...
            player->direction = ...
        }
    */
    //#########################################################################
    bool isMoving = false;
    // Player movement (vertical)
    if (keyState[ALLEGRO_KEY_W]) {
        player->coord.y -= player->speed;
        player->direction = UP;
        isMoving = true;
    }
    if (keyState[ALLEGRO_KEY_S]) {
        player->coord.y += player->speed;
        player->direction = DOWN;
        isMoving = true;
    }
    
    //###########################################################################
    // if Collide, snap to the grid to make it pixel perfect
    if(isCollision(player, map)){
        player->coord.y = round((float)original.y / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    /*
        [TODO HACKATHON 1-2] 
        
        Player Movement
        Add/Subtract the movement by player->speed

        if (keyState[ALLEGRO_KEY_A]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_D]) {
            player->coord.y = ...
            player->direction = ...

    }
    */
    //###########################################################################
    // Player movement (horizontal)
    if (keyState[ALLEGRO_KEY_A]) {
        player->coord.x -= player->speed;
        player->direction = LEFT;
        isMoving = true;
    }
    if (keyState[ALLEGRO_KEY_D]) {
        player->coord.x += player->speed;
        player->direction = RIGHT;
        isMoving = true;
    }
    //###########################################################################

    
    if(isCollision(player, map)){
        player->coord.x = round((float)original.x / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    /*
        [TODO Homework] 
        
        Calculate the animation tick to draw animation later
    */
//    //###########################################################################
     //Calculate the animation tick to draw animation later
    if (player->status == PLAYER_DYING) {
//        player->speed = 0; // to retrict player movement
        if (player->animation_tick < FRAME_DELAY * DYING_FRAMES - 1) {
            player->animation_tick++;
            printf("Dying Animation Tick: %d\n", player->animation_tick);
        }
        
    }
    else {
        player->animation_tick++;
        if (isMoving) {
            if (player->animation_tick >= FRAME_DELAY * WALKING_FRAMES) {
                player->animation_tick = 0;
            }
        }
        else {
            if (player->animation_tick >= FRAME_DELAY * IDLE_FRAMES) {
                player->animation_tick = 0;
            }
        }
    }
    ///////PLENG HAVE TO GO FIGURED OUT HOW TO CHANGE PIC and stuff
    //###########################################################################

}

void draw_player(Player * player, Point cam){
    int dy = player->coord.y - cam.y; // destiny y axis
    int dx = player->coord.x - cam.x; // destiny x axis
    
    int flag = 0; // Change the flag to flip character
    
    /*
        [TODO Homework] 
        
        Draw Animation of Dying, Walking, and Idle
    */
    //##################################
    int frame = 0;
    int frame_x = 0;
    int frame_y = 0;

    if (player->status == PLAYER_DYING) {
        frame = (player->animation_tick / FRAME_DELAY) % DYING_FRAMES;
        frame_y = 2 * FRAME_HEIGHT; // Dying animation is in the third row
    }
    else if (keyState[ALLEGRO_KEY_W] || keyState[ALLEGRO_KEY_S] || keyState[ALLEGRO_KEY_A] || keyState[ALLEGRO_KEY_D]) {
        frame = (player->animation_tick / FRAME_DELAY) % WALKING_FRAMES;
        frame_y = FRAME_HEIGHT; // Walking animation is in the second row
    }

    else {
        frame = (player->animation_tick / FRAME_DELAY) % IDLE_FRAMES;
        frame_y = 0; // Idle animation is in the first row
    }

    frame_x = frame * FRAME_WIDTH;

    
    //pleng
    if (player->knockback_CD > 0) {
        al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, 0, 0),
            frame_x, frame_y, FRAME_WIDTH, FRAME_HEIGHT, // source image x, y, width, height
            dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
            flag // Flip or not
        );
    }else {
        al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, 255, 255),
            frame_x, frame_y, FRAME_WIDTH, FRAME_HEIGHT, // source image x, y, width, height
            dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
            flag // Flip or not
        );
    }

//#####################

    
#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif
    
}

void delete_player(Player * player){
    al_destroy_bitmap(player->image);
}

static bool isCollision(Player* player, Map* map){

    if (player->coord.x < 0 ||
        player->coord.y < 0 ||
        (player->coord.x + TILE_SIZE - 1) / TILE_SIZE >= map->col ||
        (player->coord.y + TILE_SIZE - 1) / TILE_SIZE >= map->row)
        return true;
//    sawasdee. hello world :-))) jubjub everyone
    /*

        [TODO HACKATHON 2-1] 
    
        Check every corner of enemy if it's collide or not

        We check every corner if it's collide with the wall/hole or not
        You can activate DRAW_HITBOX in utility.h to see how the collision work

        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;

    */
    //###########################################################################
    // Check if the corners of the player collide with non-walkable tiles
    int left = player->coord.x / TILE_SIZE;
    int right = (player->coord.x + TILE_SIZE - 1) / TILE_SIZE;
    int top = player->coord.y / TILE_SIZE;
    int bottom = (player->coord.y + TILE_SIZE - 1) / TILE_SIZE;

    if (!isWalkable(map->map[top][left])) return true;    // Top-left
    if (!isWalkable(map->map[top][right])) return true;   // Top-right
    if (!isWalkable(map->map[bottom][left])) return true; // Bottom-left
    if (!isWalkable(map->map[bottom][right])) return true; // Bottom-right
    

    
    //###########################################################################
    
    return false;
}

void hitPlayer(Player * player, Point enemy_coord, int damage){
    if(player->knockback_CD == 0){
        float dY = player->coord.y - enemy_coord.y;
        float dX = player->coord.x - enemy_coord.x;
        float angle = atan2(dY, dX);
        
        /*
         [TODO Homework]
         
         Game Logic when the player get hit or die
         
         player->knockback_angle = angle;
         player->knockback_CD = 32;
         
         player->health = ...
         if(player->health <= 0){
         
         player->health = 0;
         player->status = ...
         
         ...
         }
         */
        //#############################################################################
        // Apply knockback effect
        player->knockback_angle = angle;
        player->knockback_CD = 32; // Cooldown for knockback effect
        
        // Reduce player's health
        player->health -= damage;
        
        // Check if player is dead
        if(player->health <= 0){
            player->health = 0;            // Prevent negative health
            player->status = PLAYER_DYING; // Set player status to DYING
        }
        //
        //if its collision with the trophy
        //#############################################################################
    }
}

// add hit trophy function
void hitPlayer_Trophy(Player * player, Point enemy_coord, int damage){
    
    player-> win = 1;
    


}


