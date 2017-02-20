#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "player.h"

Player::Player() {
    color = al_map_rgb(255, 255, 255);
    velocity = 10;
}
void Player::draw() {
    if(length > 34){
        al_draw_scaled_bitmap(assets, 144, 175, 17, 20, x, y, 17, 20, 0);
        al_draw_scaled_bitmap(assets, 161, 175, 1, 20, x + 17, y, length - 34, 20, 0);
        al_draw_scaled_bitmap(assets, 191, 175, 17, 20, x + length - 17, y, 17, 20, 0);
    }else{
        al_draw_scaled_bitmap(assets, 144, 175, 64, 20, x, y, length, 30, 0);
    }
}
void Player::moveLeft() {
    if(x - velocity > 0) {
        x = x - velocity;
    } else
        x = 0;
}
void Player::moveRight() {
    if(x + velocity < screen_width - length) {
        x = x + velocity;
    } else
        x = screen_width - length;
}
void Player::update() {
    if(isMovingLeft) {
        moveLeft();
    }
    if(isMovingRight) {
        moveRight();
    }
}
