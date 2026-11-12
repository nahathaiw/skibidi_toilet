#include <string.h>
#include "game_scene.h"
#include "menu_scene.h"
#include "loading_scene.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "weapon.h"
#include "trophy.h"
#include "UI.h"
#include "winning_scene.h"//PLENG
#include "losing_scene.h"//PLENG
#include <math.h>

#include "utility.h"

//####################
static ALLEGRO_BITMAP* heart_bitmap;
static ALLEGRO_BITMAP* coin_icon_bitmap;
static int timer_countdown = 300; // 5 seconds, assuming the game runs at 60 FPS

static int elapsed_time;  //a variable to keep track of the elapsed time:
//static void log_completion_time(double elapsed_time);// to log the completion time
//################

Player player; // Player
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet List
trophyNode * trophyList; // PLENG TROPHY

// Weapon
Weapon weapon; 
int coins_obtained;

static void init(void){
//    elapsed_time = 0;// start the time with 0

    initEnemy();
    initTrophy(); // PLENG Trophy
    map = create_map("map0.txt", 0);

    player = create_player("panda2.png", map.Spawn.x, map.Spawn.y);

    enemyList = createEnemyList();
    trophyList = createTrophyList(); // PLENG ADD trophy
    bulletList = createBulletList();
    // first weaphon
    weapon = create_weapon("guns.png", "yellow_bullet.png", 16, 8, 10); // PLENG TRY GUNS
    
    for(int i=0; i<map.EnemySpawnSize; i++){
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }
    //################################
    //for the trophy
    for(int i=0; i<map.TrophySpawnSize; i++){
        Trophy trophy = createTrophy(map.TrophySpawn[i].x, map.TrophySpawn[i].y, map.TrophyCode[i]);
        insertTrophyList(trophyList, trophy);
    }
    
    heart_bitmap = al_load_bitmap("heart.png");
    if (!heart_bitmap) {
        game_abort("Failed to load heart bitmap");
    }
    
    //for init coin bitmap
    coin_icon_bitmap = al_load_bitmap("coin_icon.png");
    if (!coin_icon_bitmap) {
        game_abort("Failed to load coin bitmap");
    }
    
    //################################

    game_log("coord x:%d \n coords y:%d \n", map.Spawn.x, map.Spawn.y);
    change_bgm("game_bgm.ogg");
}

static void update(void){

    //####################TIMER####

    elapsed_time++;

    update_player(&player, &map);

    Point Camera;
    /*
        [TODO HACKATHON 1-3]
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Adjust it based on player position variable, then subtract it with half of the gameplay screen
    */
    //###########################################################################

    Camera.x = player.coord.x - SCREEN_W / 2;
    Camera.y = player.coord.y - SCREEN_H / 2;

    //###########################################################################

   // Camera = (Point){ 0, 0 }; get rid of this so we can walk
    
    
    //CHANGE GUNS###########################################################################
    // Switch weapons based on key press
    if (keyState[ALLEGRO_KEY_1]) {
        player.currentGun = 1;
        weapon = create_weapon("guns.png", "yellow_bullet.png", 16, 8, 10);
        game_log("Switched to Gun 1");
    }
    else if (keyState[ALLEGRO_KEY_2]) {
        player.currentGun = 2;
        weapon = create_weapon("sniper.png", "orange_bullet.png", 30, 15, 25);
        game_log("Switched to Gun 2");
    }
    
    
    
    
    //  #####################################################
    
    //update enymies
    updateEnemyList(enemyList, &map, &player);
    //update trophy list
    updateTrophyList(trophyList, &map, &player);//pleng for trophy
    // Update bullets and map
    update_weapon(&weapon, bulletList, player.coord, Camera);
    updateBulletList(bulletList, enemyList, &map);
    update_map(&map, player.coord, &coins_obtained);
    
    //###############################################################################

    if (player.health <= 0) {
        // Reduce the timer countdown
        timer_countdown--;
        coins_obtained = 0;
        // When the timer reaches 0, change to the losing scene
        if (timer_countdown <= 0) {
            elapsed_time =0; // to reset it to 0 agaib

            change_scene(create_losing_scene()); // Transition to the losing scene
            return; // Exit update to avoid further updates
        }
        return; // Prevent further updates while in transition
    }
    
    if (player.win == 1){ //if player win == true check the hit trophy funtion
        int totaltime = elapsed_time /FPS;
        game_log("Time to catch trophy: %d seconds", totaltime);
        log_completion_time(totaltime);// this is to log time
        elapsed_time =0; // to reset it to 0 agaib
        int bankk = get_player_coins();
        save_player_coins(bankk+coins_obtained);
        //log_coins_collected(coins_obtained); // this is the log the coin collected
        //mooney+=coins_obtained;
        coins_obtained = 0;
        
        change_scene(create_winning_scene());
        
        // Additional logic
        return;
    }
    
    
    //###########################################################################


//    update_player(&player, &map);

}

static void draw(void){
    Point Camera;
    /*
        [TODO HACKATHON 1-4]
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Just copy from the [TODO sATHON 1-3]
    */
    //###########################################################################

    Camera.x = player.coord.x - SCREEN_W / 2;
    Camera.y = player.coord.y - SCREEN_H / 2;
    
    //###########################################################################

    //Camera = (Point){ 0, 0 }; get rid of it so we can move and walk
     
    // Draw
    draw_map(&map, Camera);
    drawEnemyList(enemyList, Camera);
    drawTrophyList(trophyList, Camera); // PLENG ADD TROPHY
    drawBulletList(bulletList, Camera);
    draw_player(&player, Camera);
    draw_weapon(&weapon, player.coord, Camera);
    
    //####DRAW TIME#####
    
    // Display the elapsed time on the top-right corner
    char timer_text[50];
    sprintf(timer_text, "Time: %d", elapsed_time / FPS);

    // Adjust the x-coordinate to align with the screen's width
    int timer_x = SCREEN_W - 6; // 10 pixels padding from the right edge
    al_draw_text(P2_FONT, al_map_rgb(255, 255, 255), timer_x, 10, ALLEGRO_ALIGN_RIGHT, timer_text);
    
    //#####################################################################################

    /*
        [TODO Homework]
        
        Draw the UI of Health and Total Coins
    */
    //#####################################################################################
    /*
        Draw the UI of Health and Total Coins
    */
    // 1. Draw Health Bar
    int health_bar_width = 200;  // Total width of the health bar
    int health_bar_height = 20;  // Height of the health bar
    int health_bar_x = 100;      // X position of the health bar
    int health_bar_y = 20;       // Y position of the health bar

    // Background of the health bar (dark gray)
    al_draw_filled_rectangle(
        health_bar_x, health_bar_y,
        health_bar_x + health_bar_width, health_bar_y + health_bar_height,
        al_map_rgb(30, 30, 30) // Darker gray for a subtle background
    );

    // Health value (gradient red to yellow, proportional to player.health)
    ALLEGRO_COLOR health_color = al_map_rgb(
        255,
        (int)(255 * player.health / 100), // Gradient from red to yellow
        0
    );
    al_draw_filled_rectangle(
        health_bar_x, health_bar_y,
        health_bar_x + (health_bar_width * player.health / 100), health_bar_y + health_bar_height,
        health_color
    );

    // Outline of the health bar (white with slight transparency)
    al_draw_rectangle(
        health_bar_x, health_bar_y,
        health_bar_x + health_bar_width, health_bar_y + health_bar_height,
        al_map_rgba(255, 255, 255, 180), // Slightly transparent white
        2
    );

    // 2. Draw Heart Icon
    int heart_icon_size = 48;  // Size of the heart icon
    int heart_icon_x = health_bar_x - heart_icon_size - 10; // Left of the health bar
    int heart_icon_y = health_bar_y + (health_bar_height / 2) - (heart_icon_size / 2); // Centered vertically with the health bar

    al_draw_scaled_bitmap(
        heart_bitmap,
        0, 0, 32, 32,                     // Source dimensions
        heart_icon_x, heart_icon_y,       // Destination position
        heart_icon_size, heart_icon_size, // Scaled size
        0
    );

    // 3. Draw Total Coins
    int coins_x = health_bar_x;                // Align coins text below the health bar
    int coins_y = health_bar_y + health_bar_height + 15;

    if (P1_FONT) {
        al_draw_textf(
            P1_FONT,
            al_map_rgb(255, 255, 0),          // Yellow color
            coins_x, coins_y,
            0,
            "Coins: %d",
            coins_obtained
        );
    } else {
        game_log("Failed to load font coin for UI.");
    }
    
    // 4. Draw Coin Icon
    int coin_icon_size = 90; // Match the size of the heart icon
    int coin_icon_x = heart_icon_x; // Align horizontally with the heart icon
    int coin_icon_y = heart_icon_y + heart_icon_size + 10; // Place below the heart icon with some spacing

    al_draw_scaled_bitmap(
        coin_icon_bitmap,
        0, 0, 32, 32,                 // Source dimensions
        coin_icon_x, coin_icon_y,     // Destination position
        coin_icon_size, coin_icon_size, // Scaled size
        0
    );



    //#####################################################################################
}

static void destroy(void){
    delete_player(&player);
    delete_weapon(&weapon);
    destroy_map(&map);
    destroyBulletList(bulletList);
    destroyEnemyList(enemyList);
    destroyTrophyList(trophyList); //PLENG ADD
    terminateEnemy();
}


Scene create_game_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "game";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;
    
    return scene;
}





