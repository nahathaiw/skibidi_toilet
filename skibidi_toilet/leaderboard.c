#include <allegro5/allegro.h>
#include "leaderboard.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

static Button backButton;
static const char* LEADERBOARD_FILE = "leaderboard.txt";
static char leaderboard_entries[10][100];
static int entry_count = 0;
static void draw_last_log_time(void);// to get the last log time down lol


static float extract_time(const char* entry) {
    float time = 0.0;
    sscanf(entry, "Player: %*s Time: %f seconds", &time);
    return time;
}

static void sort_leaderboard_entries(void) {
    for (int i = 0; i < entry_count - 1; i++) {
        for (int j = 0; j < entry_count - i - 1; j++) {
            if (extract_time(leaderboard_entries[j]) > extract_time(leaderboard_entries[j + 1])) {
                char temp[100];
                strcpy(temp, leaderboard_entries[j]);
                strcpy(leaderboard_entries[j], leaderboard_entries[j + 1]);
                strcpy(leaderboard_entries[j + 1], temp);
            }
        }
    }
}

static void load_leaderboard_entries(void) {
    FILE* leaderboard = fopen(LEADERBOARD_FILE, "r");
    if (!leaderboard) {
        return;
    }

    entry_count = 0;
    while (fgets(leaderboard_entries[entry_count], sizeof(leaderboard_entries[entry_count]), leaderboard) && entry_count < 8) {
        size_t len = strlen(leaderboard_entries[entry_count]);
        if (len > 0 && leaderboard_entries[entry_count][len - 1] == '\n') {
            leaderboard_entries[entry_count][len - 1] = '\0';
        }
        entry_count++;
    }

    fclose(leaderboard);
}
static void draw_last_log_time(void) {
    char last_entry[100] = {0};
    FILE* leaderboard = fopen(LEADERBOARD_FILE, "r");
    if (!leaderboard) {
        return;
    }

    // Read the file line by line to get the last line
    while (fgets(last_entry, sizeof(last_entry), leaderboard)) {
        // Continue reading; `last_entry` will contain the last line read
    }
    fclose(leaderboard);

    // Display the last log entry
    if (strlen(last_entry) > 0) {
        al_draw_text(
            P3_FONT,
            al_map_rgb(146, 161, 185),
            SCREEN_W / 2,
            600, // Adjust this value based on the desired position
            ALLEGRO_ALIGN_CENTER,
            last_entry
        );
    }
}

static void init(void) {
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    load_leaderboard_entries();
    sort_leaderboard_entries();

}

static void update(void) {
    update_button(&backButton);

    if ((mouseState.buttons && backButton.hovered == true) || keyState[ALLEGRO_KEY_ENTER]) {
        change_scene(create_menu_scene());
    }
}

static void draw(void) {
    // Draw leaderboard title
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        150,
        ALLEGRO_ALIGN_CENTER,
        "LEADERBOARD"
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        145,
        ALLEGRO_ALIGN_CENTER,
        "LEADERBOARD"
    );

    // Draw leaderboard entries
    for (int i = 0; i < entry_count; i++) {
        al_draw_text(
            P2_FONT,
            al_map_rgb(255, 255, 255),
            SCREEN_W / 2,
            250 + i * 40,
            ALLEGRO_ALIGN_CENTER,
            leaderboard_entries[i]
        );
    }
    // Draw last log time
    draw_last_log_time();// to draw the last log time
    
    // Draw back button
    draw_button(backButton);

    // Draw button text
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

Scene create_leaderboard_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "leaderboard";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
