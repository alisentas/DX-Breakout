#include <iostream>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "ball.h"

Ball::Ball(double g_x, double g_y, int g_radius) {
    color = al_map_rgb(255, 255, 0);
    x = g_x;
    y = g_y;
    radius = g_radius;
    v_x = 7.0;
    v_y = -7.0;
}
Ball::~Ball () {

}
void Ball::draw() {
    //al_draw_filled_circle(x, y, radius, color);
    al_draw_scaled_bitmap(assets, 85, 137, 7, 7, x - radius, y - radius, 20, 20, 0);
}
void Ball::setPosition(double g_x, double g_y) {
    x = g_x;
    y = g_y;
}
void Ball::move() {
    double next_x, next_y;
    next_x = x + v_x;
    next_y = y + v_y;

    // check wall collision
    if(next_x - radius <= 0 || next_x + radius >= screen_width) {
        v_x = -v_x;
        al_play_sample(wall, 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    if(next_y - radius <= 0){
        v_y = -v_y;
        al_play_sample(wall, 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }else if(next_y + radius >= player.y) {
        if(next_x < player.x || next_x > player.x + player.length){
            game_over = true;
        }else{
            if(x < player.x + (player.length / 6)){
                v_x = -9; //81 + 16 = 95
                v_y = -4;
            }else if(x < player.x + 2 * (player.length / 6)){
                v_x = -8;   // 64 + 36 = 100
                v_y = -6;
            }else if(x < player.x + 3 * (player.length / 6)){
                v_x = -7;   // 49 + 49 = 98
                v_y = -7;
            }else if(x < player.x + 4 * (player.length / 6)){
                v_x = 7;
                v_y = -7;
            }else if(x < player.x + 5 * (player.length / 6)){
                v_x = 8;
                v_y = -6;
            }else{
                v_x = 9;
                v_y = -4;
            }
        }
    }

    if(next_y >= 0 && next_y <= screen_height / 2.0){
    // check brick collision
        if(v_y < 0) {
            int x_pos, y_pos;
            if(get_position(next_x, next_y - radius, &x_pos, &y_pos)) {
                if(brick_positions[y_pos][x_pos]) {
                    v_y = -v_y;
                    brick_positions[y_pos][x_pos]->hit(HIT_FROM_BELOW);
                }
            }
        }
        if(v_y > 0) {
            int x_pos, y_pos;
            if(get_position(next_x, next_y + radius, &x_pos, &y_pos)) {
                if(brick_positions[y_pos][x_pos]) {
                    v_y = -v_y;
                    brick_positions[y_pos][x_pos]->hit(HIT_FROM_ABOVE);
                }
            }
        }
        if(v_x < 0) {
            int x_pos, y_pos;
            if(get_position(next_x - radius, next_y, &x_pos, &y_pos)) {
                if(brick_positions[y_pos][x_pos]) {
                    v_x = -v_x;
                    brick_positions[y_pos][x_pos]->hit(HIT_FROM_RIGHT);
                }
            }
        }
        if(v_x > 0) {
            int x_pos, y_pos;
            if(get_position(next_x + radius, next_y, &x_pos, &y_pos)) {
                if(brick_positions[y_pos][x_pos]) {
                    v_x = -v_x;
                    brick_positions[y_pos][x_pos]->hit(HIT_FROM_LEFT);
                }
            }
        }
    }
    setPosition(x + v_x, y + v_y);
}
