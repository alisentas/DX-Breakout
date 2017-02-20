#ifndef BRICK_H
#define BRICK_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

extern float brick_width, brick_height;
extern int brick_count, remaining_bricks;
extern ALLEGRO_SAMPLE *bounce, *bottle, *explode, *unbreakable;
extern ALLEGRO_BITMAP *assets, *assets2;

/********************************************//**
 * \brief Types of bricks
 ***********************************************/
typedef enum {
    UNBREAKABLE,//!< Unbreakable brick
    EXPLOSIVE,  //!< This type of bricks explode surrounding bricks, including unbreakable ones
    GOLD,       //!< Yellow brick
    DIAMOND,    //!< Cyan brick
    RUBY,       //!< Red brick
    EMERALD,    //!< Green brick
    AMETHYST,   //!< Purple brick
    SAPPHIRE,   //!< Blue brick
    INVISIBLE   //!< Invisible brick, becomes visible when hit
} brick_type;

/********************************************//**
 * \brief Types of hits
 ***********************************************/
typedef enum {
    HIT_FROM_BELOW,
    HIT_FROM_ABOVE,
    HIT_FROM_LEFT,
    HIT_FROM_RIGHT,
    EXPLODE
} hit_type;

/********************************************//**
 * \brief The Brick class
 *
 * Used to create, draw, manipulate bricks on screen.
 ***********************************************/
class Brick {
public:
    int x;                  //!< X position of the brick in the brick positions array \see brick_positions
    int y;                  //!< Y position of the brick in the brick positions array \see brick_positions
    brick_type type;        //!< Bricks type
    ALLEGRO_COLOR color_1;  //!< Primary color
    ALLEGRO_COLOR color_2;  //!< Secondary, usually lighter color
    ALLEGRO_COLOR color_3;  //!< Third, usually darker color
    ALLEGRO_COLOR color_4;  //!< Fourth color for EXPLOSIVE bricks
    ALLEGRO_COLOR color_5;  //!< Fifth color for EXPLOSIVE bricks
    ALLEGRO_COLOR color_6;  //!< Sixth color for EXPLOSIVE bricks

    /********************************************//**
     * \brief Constructor function of Brick class
     *
     * \param g_x X position of the brick in the bricks pointer array
     * \param g_y Y position of the brick in the bricks pointer array
     ***********************************************/
    Brick(int g_x, int g_y, brick_type g_type);

    ~Brick();

    /********************************************//**
     * \brief Draws the brick to the display
     ***********************************************/
    void draw();

    void hit(hit_type HitType);

    void removeBrick();
};

#endif // BRICK_H
