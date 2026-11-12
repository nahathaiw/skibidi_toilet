#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "shop_scene.h"  // Include the shop scene header PLENG
#include "loading_scene.h"
#include "leaderboard.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button settingButton;
static Button shopButton; // Add a button for the shop PLENG

static ALLEGRO_BITMAP* background_image;//PLENG implemented


static void init(void) {
    settingButton = button_create(SCREEN_W / 2 - 200, 600, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    
    //shop button PLENG
    shopButton = button_create(SCREEN_W / 2 -200, 450, 400, 100, "UI_Button.png", "UI_Button_hovered.png"); // Initialize shop button PLENG
    //BACK GROUND IMG
    background_image = al_load_bitmap("menu.png");
    change_bgm("menu_bgm.ogg");
}

static void update(void) {
    update_button(&settingButton);
    update_button(&shopButton);
    if (keyState[ALLEGRO_KEY_ENTER]) {
        change_scene(create_loading_scene());
    }
    if (keyState[ALLEGRO_KEY_L]) {
        change_scene(create_leaderboard_scene()); // enter to replay again to gain coin to buy guns
    }
    
    /*
        [TODO HACKATHON 4-3] 
        
        Change scene to setting scene when the button is pressed
    */
    // Check if the mouse is hovering over the button and the left button is pressed
    
    if ((shopButton.hovered && mouseState.buttons == true) ||(keyState[ALLEGRO_KEY_I])) {
        game_log("shopbutton hover");
        change_scene(create_shop_scene()); // Navigate to shop scene PLENG
    }
    
    if ((settingButton.hovered && mouseState.buttons == true) ||(keyState[ALLEGRO_KEY_U])) {
        // Change to the setting scene when the button is clicked
        change_scene(create_setting_scene());
    }
    
}

static void draw(void) {
    //##########PLENG
    al_draw_scaled_bitmap(
                          background_image,
                          0, 0,
                          al_get_bitmap_width(background_image), al_get_bitmap_height(background_image),
                          0, 0,
                          SCREEN_W, SCREEN_H,
                          0
                          );
    //################
    // Title Text
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        225,
        ALLEGRO_ALIGN_CENTER,
        "SKIBIDI ADVENTURE" //PLENG menu scence game title
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        220,
        ALLEGRO_ALIGN_CENTER,
        "SKIBIDI ADVENTURE"//PLENG menu scence game title
    );


    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        350,
        ALLEGRO_ALIGN_CENTER,
        "PRESS [ENTER] TO PLAY"
    );
    
    // Draw shop button
    draw_button(shopButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        450 + 28 + shopButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SHOP"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        450 + 31 + shopButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SHOP"
    );
    

    // button
    draw_button(settingButton);
    // button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        600 + 28 + settingButton.hovered * 3,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        600 + 31 + settingButton.hovered * 3,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );
}

static void destroy(void) {
    destroy_button(&settingButton);
    destroy_button(&shopButton); // Destroy the shop button

}


Scene create_menu_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "menu";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
