////
////  shop_scene.c
////  skibidi_toilet
////
////  Created by Nahathai Wonganawat on 12/27/24.
////
//



#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "shop_scene.h"
#include "player.h"

static ALLEGRO_BITMAP* background_image;
static Button backButton;
static Button upgrade_buttons[2];
static const int upgrade_prices[2] = {1, 1};
static const char* upgrade_descriptions[2] = {
    "Max Health +10",
    "Max Speed +1"
};
static int total_coins = 0;

// Global variables for cooldown
static int cooldown_frames[2] = {0, 0}; // Separate cooldown counters for each upgrade


static void init(void) {
    background_image = al_load_bitmap("shop_background.png");
    if (!background_image) {
        game_abort("Failed to load shop background image");
    }

    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "UI_Button.png", "UI_Button_hovered.png");

    // Create buttons for upgrades
    for (int i = 0; i < 2; i++) {
        upgrade_buttons[i] = button_create(200, 200 + i * 120, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    }

    // Load total coins from the coin log
    total_coins = get_player_coins();

    //change_bgm("Assets/audio/shop_bgm.ogg");
}

static void update(void) {
    // Decrement cooldown counters if they are greater than 0
    for (int i = 0; i < 2; i++) {
        if (cooldown_frames[i] > 0) {
            cooldown_frames[i]--;
        }
    }
    
    update_button(&backButton);
    if ((mouseState.buttons & 1 && backButton.hovered) ||((keyState[ALLEGRO_KEY_ENTER]))) {
        change_scene(create_menu_scene());
        return;
    }

    for (int i = 0; i < 2; i++) {
        update_button(&upgrade_buttons[i]);
        if (cooldown_frames[i] == 0) {

            //        if (mouseState.buttons & 1 && upgrade_buttons[i].hovered) {
            if ((mouseState.buttons & 1 && upgrade_buttons[i].hovered) ||
                (i == 0 && keyState[ALLEGRO_KEY_H]) ||
                (i == 1 && keyState[ALLEGRO_KEY_S])) {
                
                if (total_coins >= upgrade_prices[i]) {
                    total_coins -= upgrade_prices[i];
                    save_player_coins(total_coins);
                    cooldown_frames[i] = 10; // Set cooldown to 10 frames
                    //log_coins_collected(total_coins); // Log coins after a successful purchase
                    
                    if (i == 0) {
                        //                    player_increase_max_health(10);
                        int current_health = get_player_health();
                        game_log("Current Health before upgrade: %d", current_health);

                        int newhealth = current_health + 10;
                        save_player_health(newhealth);

                        int saved_health = get_player_health();
                        game_log("New Health after saving: %d", saved_health);

//                        int newhealth =get_player_health()+10;
//                        save_player_health(newhealth);
                        game_log("Max Health upgraded by 10 MAX= %d. Remaining coins: %d", newhealth,total_coins);
                    } else if (i == 1) {
                        int newspeed =get_player_speed()+1;
                        save_player_speed(newspeed);
                        game_log("Speed upgraded by 2.MAX= %d Remaining coins: %d", newspeed,total_coins);
                    }
                } else {
                    game_log("Not enough coins for upgrade %d", i + 1);
                }
            }
        }
    }
}

static void draw(void) {
    al_draw_scaled_bitmap(
        background_image,
        0, 0,
        al_get_bitmap_width(background_image), al_get_bitmap_height(background_image),
        0, 0,
        SCREEN_W, SCREEN_H,
        0
    );

    al_draw_text(
        P1_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        30,
        ALLEGRO_ALIGN_CENTER,
        "UPGRADE YOUR PLAYER"
    );

    al_draw_text(
        P1_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        25,
        ALLEGRO_ALIGN_CENTER,
        "UPGRADE YOUR PLAYER"
    );

    for (int i = 0; i < 2; i++) {
        draw_button(upgrade_buttons[i]);

        char text[50];
        sprintf(text, "%s: %d C", upgrade_descriptions[i], upgrade_prices[i]);

        al_draw_text(
            P2_FONT,
            al_map_rgb(255, 255, 255),
            410,
            220 + i * 120,
            ALLEGRO_ALIGN_CENTER,
            text
        );
    }


    draw_button(backButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        650 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        650 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );

    char coin_text[50];
    sprintf(coin_text, "Coins: %d", total_coins);
    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 0),
        SCREEN_W - 20,
        SCREEN_H - 40,
        ALLEGRO_ALIGN_RIGHT,
        coin_text
    );
}

static void destroy(void) {
    if (background_image) {
        al_destroy_bitmap(background_image);
    }
    destroy_button(&backButton);
    for (int i = 0; i < 2; i++) {
        destroy_button(&upgrade_buttons[i]);
    }
}

Scene create_shop_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "shop";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
