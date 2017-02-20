#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>

extern int screen_width, screen_height;
extern bool isMovingLeft, isMovingRight;
extern ALLEGRO_BITMAP *assets, *assets2;

class Player {
public:
    float x, y, length, height, velocity;
    ALLEGRO_COLOR color;
    Player();
    void draw();
    void moveLeft();
    void moveRight();
    void update();
};

#endif // PLAYER_H
