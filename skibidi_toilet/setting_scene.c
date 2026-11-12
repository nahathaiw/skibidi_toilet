
#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

// Slider properties for BGM
static float bgm_slider_value = 0.5f; // Initial value for the BGM slider
static int bgm_slider_width = 400;
static int bgm_slider_height = 20;

// Slider properties for SFX
static float sfx_slider_value = 0.5f; // Initial value for the SFX slider
static int sfx_slider_width = 400;
static int sfx_slider_height = 20;

static ALLEGRO_BITMAP* background_image;
static Button backButton;

static void init(void) {
    background_image = al_load_bitmap("shop_background.png");
    if (!background_image) {
        game_abort("Failed to load shop background image");
    }

    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "UI_Button.png", "UI_Button_hovered.png");

    bgm_slider_value = BGM_VOLUME; // Sync BGM slider with initial BGM volume
    sfx_slider_value = SFX_VOLUME; // Sync SFX slider with initial SFX volume
}

static void update(void) {
    update_button(&backButton);

    if (mouseState.buttons & 1) { // Check if the left mouse button is pressed
        // Handle BGM slider adjustment
        int bgm_slider_x = (SCREEN_W - bgm_slider_width) / 2;
        int bgm_slider_y = 300;
        if (mouseState.x >= bgm_slider_x && mouseState.x <= bgm_slider_x + bgm_slider_width &&
            mouseState.y >= bgm_slider_y && mouseState.y <= bgm_slider_y + bgm_slider_height) {
            bgm_slider_value = (float)(mouseState.x - bgm_slider_x) / bgm_slider_width;

            if (bgm_slider_value < 0.0f) bgm_slider_value = 0.0f;
            if (bgm_slider_value > 1.0f) bgm_slider_value = 1.0f;

            BGM_VOLUME = bgm_slider_value;
            if (bgm_instance) {
                al_set_sample_instance_gain(bgm_instance, BGM_VOLUME);
            }
        }

        // Handle SFX slider adjustment
        int sfx_slider_x = (SCREEN_W - sfx_slider_width) / 2;
        int sfx_slider_y = 400;
        if (mouseState.x >= sfx_slider_x && mouseState.x <= sfx_slider_x + sfx_slider_width &&
            mouseState.y >= sfx_slider_y && mouseState.y <= sfx_slider_y + sfx_slider_height) {
            sfx_slider_value = (float)(mouseState.x - sfx_slider_x) / sfx_slider_width;

            if (sfx_slider_value < 0.0f) sfx_slider_value = 0.0f;
            if (sfx_slider_value > 1.0f) sfx_slider_value = 1.0f;

            SFX_VOLUME = sfx_slider_value;
        }
    }

    if ((mouseState.buttons & 1 && backButton.hovered) ||((keyState[ALLEGRO_KEY_ENTER]))) {
        change_scene(create_menu_scene());
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
        al_map_rgb(0, 0, 185),
        SCREEN_W / 2,
        30,
        ALLEGRO_ALIGN_CENTER,
        "SOUND EFFECT"
    );

    al_draw_text(
        P1_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        25,
        ALLEGRO_ALIGN_CENTER,
        "SOUND EFFECT"
    );

    // Draw SOUND label
    al_draw_text(
        P1_FONT,
        al_map_rgb(0, 0, 255),
        SCREEN_W / 2,
        250,
        ALLEGRO_ALIGN_CENTER,
        "SOUND"
    );



    // Draw the BGM slider
    int bgm_slider_x = (SCREEN_W - bgm_slider_width) / 2;
    int bgm_slider_y = 300;
    al_draw_filled_rectangle(bgm_slider_x, bgm_slider_y, bgm_slider_x + bgm_slider_width, bgm_slider_y + bgm_slider_height,
                              al_map_rgb(200, 200, 200));
    int bgm_handle_x = bgm_slider_x + (int)(bgm_slider_value * bgm_slider_width);
    al_draw_filled_rectangle(bgm_handle_x - 5, bgm_slider_y - 10, bgm_handle_x + 5, bgm_slider_y + bgm_slider_height + 10,
                              al_map_rgb(100, 100, 255));

    // Draw the EFFECT label
    al_draw_text(
        P2_FONT,
        al_map_rgb(0, 0, 225),
        SCREEN_W / 2,
        350,
        ALLEGRO_ALIGN_CENTER,
        "EFFECT"
    );

    // Draw the SFX slider
    int sfx_slider_x = (SCREEN_W - sfx_slider_width) / 2;
    int sfx_slider_y = 400;
    al_draw_filled_rectangle(sfx_slider_x, sfx_slider_y, sfx_slider_x + sfx_slider_width, sfx_slider_y + sfx_slider_height,
                              al_map_rgb(200, 200, 200));
    int sfx_handle_x = sfx_slider_x + (int)(sfx_slider_value * sfx_slider_width);
    al_draw_filled_rectangle(sfx_handle_x - 5, sfx_slider_y - 10, sfx_handle_x + 5, sfx_slider_y + sfx_slider_height + 10,
                              al_map_rgb(100, 255, 100));

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
}

static void destroy(void) {
    destroy_button(&backButton);
}

Scene create_setting_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "setting";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
