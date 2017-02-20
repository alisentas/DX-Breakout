#include <iostream>
#include "version.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "brick.h"
#include "player.h"
#include "ball.h"

#define FPS_LIMIT 60                        //!< FPS limit of the game. Integrated intel chipset handles 60 FPS, Nvidia handles 85

int screen_width;                           //!< Screen width
int screen_height;                          //!< Screen height
ALLEGRO_DISPLAY *display = NULL;            //!< Default display
ALLEGRO_KEYBOARD_STATE keyboard_state;      //!< Keyboard state
ALLEGRO_FONT *ubuntu16;                     //!< 16px Ubuntu Font
ALLEGRO_EVENT_QUEUE *queue;                 //!< Event queue
ALLEGRO_TIMER *timer;                       //!< Timer, fires timer interrupts per 1/FPS
float brick_width;                          //!< Width of an individual brick
float brick_height;                         //!< Height of an individual brick
int brick_count;                            //!< Count of bricks shown on screen
int remaining_bricks;                       //!< Remaining bricks needed to finish the game
bool isMovingLeft = false;                  //!< Indicates if A key is pressed down
bool isMovingRight = false;                 //!< Indicates if D key is pressed down
Player player;                              //!< Player object
Brick *brick_positions[15][10];             //!< Positions of bricks, shown on screen
std::vector<Brick> bricks;                  //!< Container vector of brick(s)
std::vector<Brick>::iterator brick_iterator;
std::vector<Ball> balls;                    //!< Container vector of ball(s)
std::vector<Ball>::iterator ball_iterator;
bool game_over = false;                     //!< Defines if the game is ended or not
ALLEGRO_MIXER *mixer;
ALLEGRO_VOICE *voice;
ALLEGRO_AUDIO_STREAM *background_music;
ALLEGRO_SAMPLE *bounce, *bottle, *explode, *wall, *unbreakable;
double old_time;
ALLEGRO_BITMAP *assets, *assets2, *background_image;
int selected_menu_item = 0;
bool playing;

bool get_position(float x, float y, int *x_pos, int *y_pos);
void remove_brick(int x_pos, int y_pos);
void init_game();
int game_tick();
void clear_game();
void print_debug(int);
void load_assets();
void add_ball();
int init_allegro();
void draw_main_menu();

int main() {
    init_allegro();

    ALLEGRO_DISPLAY_MODE video_mode;
    al_get_display_mode(0, &video_mode);
    screen_height = video_mode.height;
    screen_width = video_mode.width;

    load_assets();

    // create new display
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    display = al_create_display(screen_width, screen_height);
    al_hide_mouse_cursor(display);

    al_start_timer(timer);
    //init_game();
    bool redraw = false, redraw_main = true;
    old_time = al_get_time();
    while(true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);
        if(ev.type == ALLEGRO_EVENT_TIMER) {
            if(!game_over){
                redraw = true;
            }
        } else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if(ev.keyboard.keycode == ALLEGRO_KEY_A) {
                isMovingLeft = true;
            }else if(ev.keyboard.keycode == ALLEGRO_KEY_D) {
                isMovingRight = true;
            }else if(ev.keyboard.keycode == ALLEGRO_KEY_W && !playing){
                selected_menu_item = (selected_menu_item - 1) % 3;
                redraw_main = true;
            }else if(ev.keyboard.keycode == ALLEGRO_KEY_S && !playing){
                selected_menu_item = (selected_menu_item + 1) % 3;
                redraw_main = true;
            }else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER && !playing){
                if(selected_menu_item == 0 && !playing){
                    init_game();
                }else if(selected_menu_item == 2 && !playing){
                    al_destroy_display(display);
                    return 0;
                }
            }

        } else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
            if(ev.keyboard.keycode == ALLEGRO_KEY_A) {
                isMovingLeft = false;
            } else if(ev.keyboard.keycode == ALLEGRO_KEY_D) {
                isMovingRight = false;
            }
        }

        if(redraw && !game_over && playing && al_event_queue_is_empty(queue)) {
            redraw = false;
            game_tick();
        }

        if(redraw_main && !playing && al_event_queue_is_empty(queue)){
            draw_main_menu();
            redraw_main = false;
        }

        if(game_over){
            game_over = false;
            redraw_main = true;
            playing = false;
            clear_game();
        }
    }

    al_destroy_display(display);

    printf("game over, bravo");

    return 0;
}

void init_game() {
    game_over = false;
    playing = true;
    // initialize player
    player.length = screen_width / 8;
    player.height = screen_height / 30;
    player.x = (screen_width / 2) - (player.length / 2);
    player.y = screen_height - player.height - 20;

    // initialize ball(s)
    balls.push_back(*(new Ball(player.x + (player.length / 2) - 10, player.y - 20, 10)));

    // initialize bricks
    brick_width = screen_width / 10;
    brick_height = (screen_height / 2) / 15;
    brick_count = 80;
    int i, j;
    for(i = 0; i < 15; ++i) {
        for(j = 0; j < 10; ++j) {
            brick_positions[i][j] = NULL;
        }
    }
    i = 0;
    while(i != brick_count) {
        while(true) {
            int x_pos = rand() % 10;
            int y_pos = rand() % 15;
            if(!brick_positions[y_pos][x_pos]) {
                Brick *brick = new Brick(x_pos, y_pos, (brick_type)(rand() % 8));
                brick_positions[y_pos][x_pos] = brick;
                bricks.push_back(*brick);
                ++i;
                break;
            }
        }
    }
}

void clear_game() {
    bricks.clear();
    balls.clear();
}

int game_tick() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    //208 232
    int i, j;
    for(i = 0; i < screen_width; i += 500){
        for(j = 0; j < screen_height; j += 500){
            al_draw_bitmap(background_image, i, j, 0);
        }
    }
    double new_time = al_get_time();
    int FPS = (int)(1/((new_time - old_time)));
    old_time = new_time;
    // draw bricks
    brick_iterator = bricks.begin();
    while(brick_iterator != bricks.end()) {
        (*brick_iterator).draw();
        brick_iterator++;
    }
    // draw balls
    ball_iterator = balls.begin();
    while(ball_iterator != balls.end()) {
        (*ball_iterator).draw();
        (*ball_iterator).move();
        ball_iterator++;
    }
    player.draw();
    player.update();
    print_debug(FPS);
    al_flip_display();

    return 0;
}

void print_debug(int FPS){
    char strx[50];
    sprintf(strx, "Build: %ld v%s", AutoVersion::BUILDS_COUNT, AutoVersion::FULLVERSION_STRING);
    al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), 10, screen_height - 15, 0, strx);
    sprintf(strx, "FPS: %d", FPS);
    al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), 10, screen_height - 30, 0, strx);
    sprintf(strx, "Brick Count: %ld", bricks.size());
    al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), 10, screen_height - 45, 0, strx);
    sprintf(strx, "Ball Count: %ld", balls.size());
    al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), 10, screen_height - 60, 0, strx);
}

/********************************************//**
 * \brief Gets the position of a brick given x,y coordinates
 *
 * This function changes the value of \p *x_pos and \p *y_pos according to the value
 * \p x \p y given by user.
 * \code
 * get_position(150, 410, &x, &y);
 * \endcode
 *
 * \param x X position of the ball
 * \param y Y position of the ball
 * \param *x_pos The variable which will be the X position of brick in given coordinates
 * \param *y_pos The variable which will be the Y position of brick in given coordinates
 * \return bool Returns true if there can be brick in the given position, false otherwise
 *
 ***********************************************/
bool get_position(float x, float y, int *x_pos, int *y_pos) {
    *x_pos = (int)(x / brick_width);
    *y_pos = (int)(y / brick_height);
    if(*x_pos < 0 || *x_pos > 9 || *y_pos < 0 || *y_pos > 14) {
        return false;
    }
    return true;
}

int init_allegro(){
    srand(time(NULL));
    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }
    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize keyboard!\n");
        return -1;
    }
    if(!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }
    if(!al_init_image_addon()){
        fprintf(stderr, "failed to initialize image addon\n");
        return -1;
    }
    if(!al_init_font_addon()) {
        fprintf(stderr, "failed to initialize font addon!\n");
        return -1;
    }
    if(!al_init_ttf_addon()) {
        fprintf(stderr, "failed to initialize ttf addon!\n");
        return -1;
    }
    if(!al_install_audio()) {
        fprintf(stderr, "failed to install audio!\n");
        return -1;
    }
    if(!al_init_acodec_addon()) {
        fprintf(stderr, "failed to initialize acodec addon!\n");
        return -1;
    }

    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    if(!voice) {
        std::cout << "Error creating voice." << std::endl;
    }
    mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    if(!mixer) {
        std::cout << "Error creating mixer." << std::endl;
    }

    al_reserve_samples(8);

    timer = al_create_timer(1.0 / FPS_LIMIT);

    if(!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    queue = al_create_event_queue();
    if(!queue) {
        fprintf(stderr, "failed to create event queue!\n");
        return -1;
    }
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    return 0;
}

void add_ball(){
    Ball ball(player.x + (player.length / 2) - 10, player.y - 20, 10);
    balls.push_back(ball);
}

void load_assets(){
    ubuntu16 =  al_load_ttf_font("res/fonts/ubuntu.ttf", 16, 0);

    assets = al_load_bitmap("res/images/pieces.png");
    background_image = al_load_bitmap("res/images/background.png");

    bottle = al_load_sample("res/sound/bottle.wav");
    explode = al_load_sample("res/sound/explode.wav");
    wall = al_load_sample("res/sound/wall.wav");
    unbreakable = al_load_sample("res/sound/unbreakable.wav");

    // background music by Joseph Gilbert / Kistol
    // from htp://opengameart.org/content/snowfall
    background_music = al_load_audio_stream("res/sound/snowfall.ogg", 4, 2048);

    al_set_audio_stream_playmode(background_music, ALLEGRO_PLAYMODE_LOOP);
    if(!background_music) {
        std::cout << "Error loading background music!" << std::endl;
    }
    if(!al_attach_mixer_to_voice(mixer, voice)) {
        std::cout << "Error attaching mixer to voice." << std::endl;
    }
    if(!al_attach_audio_stream_to_mixer(background_music, mixer)) {
        std::cout << "Error attaching audio stream to mixer." << std::endl;
    }
}

void draw_main_menu(){
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if(selected_menu_item == 0)
        al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), screen_width / 2 - 50, screen_height / 2 - 16, 0, "> Start Game");
    else
        al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), screen_width / 2 - 50, screen_height / 2 - 16, 0, "  Start Game");
    if(selected_menu_item == 1)
        al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), screen_width / 2 - 50, screen_height / 2, 0,      "> Credits");
    else
        al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), screen_width / 2 - 50, screen_height / 2, 0,      "  Credits");
    if(selected_menu_item == 2)
        al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), screen_width / 2 - 50, screen_height / 2 + 16, 0, "> Quit");
    else
        al_draw_text(ubuntu16, al_map_rgb(255, 255, 255), screen_width / 2 - 50, screen_height / 2 + 16, 0, "  Quit");
    al_flip_display();
}
