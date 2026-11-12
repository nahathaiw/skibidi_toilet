#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "utility.h"

const int FPS = 60; 
const int SCREEN_W = 800;
const int SCREEN_H = 800;
const int RESERVE_SAMPLES = 10;
const char* GAME_TITLE = "Skibidi Adventure"; //PLENG changed this
const char* log_file = "log.txt";
const char* font_file = "Minecraft.ttf";
const int TILE_SIZE = 64;

//#################PLENG
ALLEGRO_SAMPLE* BGM = NULL;
float SFX_VOLUME = 0.5f;// is the the volume slider part
float BGM_VOLUME = 0.5f;

ALLEGRO_SAMPLE_INSTANCE* bgm_instance = NULL; //to change the bgm part
void log_completion_time(double elapsed_time) ;//for log leaderboard
void create_log_file();
int get_last_log_time();//#################
static int elapsed_time_log_count = 0; // count how many time

void log_coins_collected(int coins_collected);
int get_total_logged_coins();
void log_player_health(int health);
void log_player_speed(float speed);



// Global variable to hold coins
int player_coins = 0;
int player_speed = 2;
int player_health = 50;
void save_player_coins(int coins);
int get_player_coins(void);

ALLEGRO_FONT* TITLE_FONT;
ALLEGRO_FONT* P1_FONT;
ALLEGRO_FONT* P2_FONT;
ALLEGRO_FONT* P3_FONT;


//################PLENG AUDIO########
void change_bgm(char* audio_path) {
    if (bgm_instance) {
        al_stop_sample_instance(bgm_instance);
        al_destroy_sample_instance(bgm_instance);
        bgm_instance = NULL;
    }

    BGM = al_load_sample(audio_path);
    if (!BGM) {
        game_log("No BGM File found [%s], no sound will be played", audio_path);
    } else {
        bgm_instance = al_create_sample_instance(BGM);
        al_attach_sample_instance_to_mixer(bgm_instance, al_get_default_mixer());
        al_set_sample_instance_gain(bgm_instance, BGM_VOLUME);
        al_play_sample_instance(bgm_instance);
    }
}
//################PLENG CHANGE########


void init_Util(void){
    TITLE_FONT = al_load_font("Minecraft.ttf", 64, 0);
    if(!TITLE_FONT){
        game_abort("Failed to load Title Font");
    }
    
    P1_FONT = al_load_font(font_file, 48, 0);
    if(!P1_FONT){
        game_abort("Failed to load P1 Font");
    }
    
    P2_FONT = al_load_font(font_file, 36, 0);
    if(!P2_FONT){
        game_abort("Failed to load P2 Font");
    }
    
    P3_FONT = al_load_font(font_file, 24, 0);
    if(!P3_FONT){
        game_abort("Failed to load P3 Font");
    }
}

void destroy_init(void) {
    al_destroy_font(TITLE_FONT);
    al_destroy_font(P1_FONT);
    al_destroy_font(P2_FONT);
    al_destroy_font(P3_FONT);
}

///////////////PLENG TRY TO SAVE THE FILE


/*
    [GAME LOG]

    You can disable the log by remove the "#define LOG_ENABLE" in utility.h
    It's not recommended if you are still in progress on making this game
    You may do it after finishing it for performance boost purpose
*/
void print_log(const char * msg, va_list arg, logtype type){
#ifdef LOG_ENABLE
    // Open File
    static bool firstOpen = true;
    FILE* f;

    if (firstOpen) {
        f = fopen(log_file, "w");
        firstOpen = false;
    }
    else {
        f = fopen(log_file, "a");
    }
    
    switch(type){
        case Log:
            printf("[LOG] ");
            if(f != NULL)
                fprintf(f, "[LOG] ");
            break;
        case Error:
            printf("[Error] ");
            if(f != NULL)
                fprintf(f, "[Error] ");
            break;
        case Warning:
            printf("[Warning] ");
            if(f != NULL)
                fprintf(f, "[Warning] ");
        default:
            printf("[???] ");
            if(f != NULL)
                fprintf(f, "[???] ");
            break;
    }
    
    vprintf(msg, arg);
    printf("\n");
    if(f != NULL){
        vfprintf(f, msg, arg);
        fprintf(f, "\n");
        fclose(f);
    }

#endif
}

void game_log(const char * msg, ...){
#ifdef LOG_ENABLE
    va_list args;
    va_start(args, msg);
    print_log(msg, args, Log);
    va_end(args);
#endif
}

void game_abort(const char * msg, ...){
#ifdef LOG_ENABLE
    va_list args;
    va_start(args, msg);
    print_log(msg, args, Error);
    va_end(args);
#endif
    // Abort the game if there is an error
    exit(1);
}

// To print with a Warning Sign
void game_warning(const char * msg, ...){
#ifdef LOG_ENABLE
    va_list args;
    va_start(args, msg);
    print_log(msg, args, Warning);
    va_end(args);
#endif
}
//#################
#include <stdio.h>
#include <unistd.h>

void print_file_contents(const char *file_path) {
    printf("Checking file at path: %s\n", file_path);

    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    printf("Contents of %s:\n", file_path);

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); // Print each line of the file
    }

    fclose(file);
    printf("\n--- End of File ---\n");
}




void log_completion_time(double elapsed_time) {
    elapsed_time_log_count++;

    // Get the current working directory
    char *cwd = getcwd(NULL, 0);
    if (!cwd) {
        perror("Error getting current working directory");
        return;
    }

    printf("Current working directory: %s\n", cwd);

    // Construct the full path to the leaderboard file
    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s/%s", cwd, "leaderboard.txt");

    // Open the leaderboard file in append mode
    FILE *leaderboard = fopen(file_path, "a");
    if (!leaderboard) {
        perror("Error opening leaderboard file");
        free(cwd); // Free the allocated memory for `getcwd`
        return;
    }
    
    
    // Log the elapsed time
    fprintf(leaderboard, "Player: %s Time: %.2f seconds\n", "Anonymous", elapsed_time);

    fclose(leaderboard);

    printf("Logged elapsed time: %.2f seconds to %s\n", elapsed_time, file_path);
    print_file_contents(file_path); // Call to print the file contentss

    // Free the allocated memory for `getcwd`
    free(cwd);
}

#include <stdio.h>
#include <string.h>



// Function to get the last log entry as an integer (elapsed time)
int get_last_log_time() {
    FILE *leaderboard = fopen("leaderboard.txt", "r");
    if (!leaderboard) {
        perror("Error opening leaderboard file");
        return -1; // Indicating an error
    }

    char line[256];
    char last_line[256] = "";

    // Read each line and store the last non-empty line
    while (fgets(line, sizeof(line), leaderboard)) {
        if (strlen(line) > 0) {
            strcpy(last_line, line);
        }
    }

    fclose(leaderboard);

    if (strlen(last_line) > 0) {
        int elapsed_time = -1;
        // Extract the time from the last line
        sscanf(last_line, "Player: %*s Time: %d", &elapsed_time);
        return elapsed_time;
    } else {
        return -1; // Indicating no data found
    }
}





//NOW FOR THE COIN LOGGING FUNCTION

void save_player_speed(int speed) {
    player_speed = speed;
    // Save to the file in the "Assets" folder
    FILE *file = fopen("speed.txt", "w");
    if (file) {
        fprintf(file, "%d", speed);
        fclose(file);
        game_log("save succesfullu");
    } else {
        game_log("Failed to save player coins to file.");
    }
}

void save_player_health(int health) {
    player_health = health;
    // Save to the file in the "Assets" folder
    FILE *file = fopen("health.txt", "w");
    if (file) {
        fprintf(file, "%d", health);
        fclose(file);
        game_log("save succesfulluy");
    } else {
        game_log("Failed to save player coins to file.");
    }
}

void save_player_coins(int coins) {
    player_coins = coins;
    // Save to the file in the "Assets" folder
    FILE *file = fopen("bank.txt", "w");
    if (file) {
        fprintf(file, "%d", coins);
        fclose(file);
        game_log("save succesfullu ");
    } else {
        game_log("Failed to save player coins to file.");
    }
}

int get_player_speed(void) {
    if (player_speed < 0) { // Check if coins are uninitialized
        // Read from the file in the "Assets" folder
        FILE *file = fopen("speed.txt", "r");
        if (file) {
            int last_speed = 0;
            while (fscanf(file, "%d", &last_speed) == 1) {
                // Keep reading until EOF to get the last number
            }
            fclose(file);
            player_speed = last_speed;
        } else {
            player_speed = 2; // Default to 0 if file doesn't exist
        }
    }
    return player_speed;
}

int get_player_health(void) {
    if (player_health < 0) { // Check if coins are uninitialized
        // Read from the file in the "Assets" folder
        FILE *file = fopen("speed.txt", "r");
        if (file) {
            int last_health = 50;
            while (fscanf(file, "%d", &last_health) == 1) {
                // Keep reading until EOF to get the last number
            }
            fclose(file);
            player_health = last_health;
        } else {
            player_health = 50; // Default to 0 if file doesn't exist
        }
    }
    return player_health;
}


int get_player_coins(void) {
    if (player_coins == 0) { // Check if coins are uninitialized
        // Read from the file in the "Assets" folder
        FILE *file = fopen("bank.txt", "r");
        if (file) {
            int last_coins = 0;
            while (fscanf(file, "%d", &last_coins) == 1) {

                // Keep reading until EOF to get the last number
            }
            fclose(file);
            player_coins = last_coins;
            game_log("Player coins %d", player_coins);
        } else {
            game_log("fail to get coin");
            player_coins = 0; // Default to 0 if file doesn't exist
        }
    }
    return player_coins;
}

