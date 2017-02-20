#include <iostream>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "brick.h"

extern std::vector<Brick> bricks;
extern Brick *brick_positions[15][10];

Brick::Brick(int g_x, int g_y, brick_type g_type) {
    x = g_x;
    y = g_y;
    type = g_type;
    if(type == EXPLOSIVE) {
        color_1 = al_map_rgb(254, 48, 0);
        color_2 = al_map_rgb(204, 48, 0);
        color_3 = al_map_rgb(170, 48, 0);
        color_4 = al_map_rgb(135, 49, 0);
        color_5 = al_map_rgb(255, 0, 0);
        color_6 = al_map_rgb(150, 0, 0);
    }
}

Brick::~Brick(){

}

void Brick::draw() {
    if(type == EXPLOSIVE) {
        al_draw_filled_rectangle(x * brick_width, y * brick_height, x * brick_width + (brick_width / 4), y * brick_height + brick_height, color_1);
        al_draw_filled_rectangle(x * brick_width + (brick_width / 4), y * brick_height, x * brick_width + 2 * (brick_width / 4), y * brick_height + brick_height, color_2);
        al_draw_filled_rectangle(x * brick_width + 2 * (brick_width / 4), y * brick_height, x * brick_width + 3 * (brick_width / 4), y * brick_height + brick_height, color_3);
        al_draw_filled_rectangle(x * brick_width + 3 * (brick_width / 4), y * brick_height, x * brick_width + brick_width, y * brick_height + brick_height, color_4);
        al_draw_filled_rectangle(x * brick_width, y * brick_height + brick_height - 5, x * brick_width + brick_width, y * brick_height + brick_height, color_6);
        al_draw_filled_rectangle(x * brick_width, y * brick_height, x * brick_width + brick_width, y * brick_height + 5, color_5);
        al_draw_filled_rectangle(x * brick_width, y * brick_height, x * brick_width + 5, y * brick_height + brick_height, color_5);
        al_draw_filled_rectangle(x * brick_width + brick_width - 5, y * brick_height, x * brick_width + brick_width, y * brick_height + brick_height, color_6);
        al_draw_rectangle(x * brick_width, y * brick_height, x  * brick_width + brick_width, y * brick_height + brick_height, al_map_rgb(0, 0, 0), 2);
        return;
    }
    if(type == GOLD){
        al_draw_scaled_bitmap(assets, 116, 92, 64, 16, x * brick_width, y * brick_height, brick_width, brick_height, 0);
        return;
    }else if(type == UNBREAKABLE){
        al_draw_scaled_bitmap(assets, 116, 112, 64, 16, x * brick_width, y * brick_height, brick_width, brick_height, 0);
        return;
    }else if(type == EMERALD){
        al_draw_scaled_bitmap(assets, 48, 92, 64, 16, x * brick_width, y * brick_height, brick_width, brick_height, 0);
        return;
    }else if(type == AMETHYST){
        al_draw_scaled_bitmap(assets, 116, 72, 64, 16, x * brick_width, y * brick_height, brick_width, brick_height, 0);
        return;
    }else if(type == DIAMOND){
        al_draw_scaled_bitmap(assets, 48, 72, 64, 16, x * brick_width, y * brick_height, brick_width, brick_height, 0);
        return;
    }else if(type == SAPPHIRE){
        al_draw_scaled_bitmap(assets, 184, 72, 64, 16, x * brick_width, y * brick_height, brick_width, brick_height, 0);
        return;
    }else if(type == RUBY){
        al_draw_scaled_bitmap(assets, 47, 112, 64, 16, x * brick_width, y * brick_height, brick_width, brick_height, 0);
        return;
    }
}

void Brick::hit(hit_type hitType){
    if(type == UNBREAKABLE){
        al_play_sample(unbreakable, 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        if(hitType == EXPLODE){
            removeBrick();
        }
    }else if(type == EXPLOSIVE){
        al_play_sample(explode, 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        int i, j;
        removeBrick();
        for(i = x - 1; i < x + 2; ++i){
            for(j = y - 1; j < y + 2; ++j){
                if(i == x && j == y){
                    continue;
                }
                if(i >= 0 && i < 10 && j >= 0 && j < 15){
                    if(brick_positions[j][i]){
                        brick_positions[j][i]->hit(EXPLODE);
                    }
                }
            }
        }
    }else if(type >= GOLD && type <= SAPPHIRE){
        al_play_sample(bottle, 1.0, 0.5, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        removeBrick();
    }
}

void Brick::removeBrick(){
    std::vector<Brick>::iterator brick_i = bricks.begin();
    while(brick_i != bricks.end()){
        if(brick_i->x == x && brick_i->y == y) {
            bricks.erase(brick_i);
            break;
        }
        ++brick_i;
    }
    brick_positions[y][x] = NULL;
}
