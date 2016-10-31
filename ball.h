#ifndef BALL_H
#define BALL_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include "brick.h"
#include "player.h"

extern int screen_height, screen_width;
extern bool get_position(float x, float y, int *x_pos, int *y_pos);
extern void remove_brick(int x_pos, int y_pos);
extern Brick *brick_positions[15][10];
extern Player player;
extern bool game_over;
extern ALLEGRO_FONT *ubuntu16;
extern ALLEGRO_SAMPLE *bounce, *bottle, *explode, *wall;
extern ALLEGRO_BITMAP *assets, *assets2;

/********************************************//**
 * \brief The Ball class
 *
 * Used to create, manipulate and draw balls
 ***********************************************/
class Ball {
public:
    double x;                //!< X position of the ball on screen
    double y;                //!< Y position of the ball on screen
    int radius;           //!< Radius of the ball
    double v_x;              //!< X velocity of the ball, used to calculate next position
    double v_y;              //!< Y velocity of the ball, used to calculate next position
    ALLEGRO_COLOR color;    //!< Color of the ball

    /********************************************//**
     * \brief Constructor function of the ball
     *
     * \param g_x Given X coordinates on screen
     * \param g_y Given Y coordinates on screen
     * \param g_radius Given radius of the ball
     ***********************************************/
    Ball(double g_x, double g_y, int g_radius);

    /********************************************//**
     * \brief Destructor of the ball
     ***********************************************/
    ~Ball ();

    /********************************************//**
     * \brief Used to draw the ball on screen
     ***********************************************/
    void draw();

    /********************************************//**
     * \brief Sets ball position to the given coordinates
     *
     * \param g_x desired X position
     * \param g_y Desired Y position
     ***********************************************/
    void setPosition(double g_x, double g_y);
    void move();
};

#endif // BALL_H
