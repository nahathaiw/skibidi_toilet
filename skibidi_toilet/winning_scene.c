#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "loading_scene.h"
#include "winning_scene.h"// pleng
#include "leaderboard.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button settingButton;
static ALLEGRO_BITMAP* panda_win_bitmap;


static void init(void) {
    settingButton = button_create(SCREEN_W / 2 - 200, 600, 400, 100, "UI_Button.png", "UI_Button_hovered.png");

    change_bgm("menu_bgm.ogg");
    //##################################################
    panda_win_bitmap = al_load_bitmap("panda_win.png");
    if (!panda_win_bitmap) {
        game_abort("Failed to load player bitmap");
    }

    change_bgm("None");
    //########################################
}

static void update(void) {
    update_button(&settingButton);
    if (keyState[ALLEGRO_KEY_ENTER]) {
        change_scene(create_menu_scene()); // enter to replay again to gain coin to buy guns
    }
    if (keyState[ALLEGRO_KEY_L]) {
        change_scene(create_leaderboard_scene()); // enter to replay again to gain coin to buy guns
    }

    /*
        [TODO HACKATHON 4-3]
        
        Change scene to setting scene when the button is pressed
    */
    // Check if the mouse is hovering over the button and the left button is pressed
    if (settingButton.hovered && mouseState.buttons == true) {
        // Change to the setting scene when the button is clicked
        change_scene(create_menu_scene());
    }
    
}

static void draw(void) {
    // Title Text
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        225,
        ALLEGRO_ALIGN_CENTER,
        "YOU WIN" //PLENG menu scence game title
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        220,
        ALLEGRO_ALIGN_CENTER,
        "YOU WIN"//PLENG menu scence game title
    );

    // Display last completion time at the middle top
    int last_time = get_last_log_time();
    char time_text[64];
    if (last_time != -1) {
        snprintf(time_text, sizeof(time_text), "Completion Time: %d seconds", last_time);
    } else {
        snprintf(time_text, sizeof(time_text), "Completion Time: Not Available");
    }
    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        50, // Middle top position
        ALLEGRO_ALIGN_CENTER,
        time_text
    );
    // button
    draw_button(settingButton);
    // button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        600 + 28 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "HOME"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        600 + 31 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "HOME"
    );
    ///this is for panda_win picture
    // Centered and Scaled Panda Win Bitmap
    int panda_width = al_get_bitmap_width(panda_win_bitmap);
    int panda_height = al_get_bitmap_height(panda_win_bitmap);
    float scale_factor = 3.0; // Scale up by 3 times
    int scaled_width = panda_width * scale_factor;
    int scaled_height = panda_height * scale_factor;
    int panda_x = (SCREEN_W - scaled_width) / 2;
    int panda_y = (SCREEN_H - scaled_height) / 2;

    al_draw_scaled_bitmap(
        panda_win_bitmap,
        0, 0, panda_width, panda_height, // Source dimensions
        panda_x, panda_y,                // Destination position (centered)
        scaled_width, scaled_height,     // Scaled size
        0                                // Flags
    );
}

static void destroy(void) {
    destroy_button(&settingButton);
}


Scene create_winning_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "winning";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
