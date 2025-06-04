#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include "player.h"

player::~player()
{
	al_destroy_bitmap(image);
}
player::player(int HEIGHT)
{

	image = al_create_bitmap(64,64);   
	if(!image) {
		exit(1);
	}

	al_set_target_bitmap(image);
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_filled_rectangle(0,25,64,39,al_map_rgb(75, 75, 75));
	al_draw_filled_rectangle(25,0,39,64,al_map_rgb(50, 50, 50));		
	al_draw_circle(32,32,8,al_map_rgb(0, 0, 0),5);

	al_draw_line(0,32,64,32,al_map_rgb(255, 100, 255),2);
	al_draw_line(32,0,32,64,al_map_rgb(255, 100, 255),2);
	al_draw_circle(32,32,16,al_map_rgb(200, 200, 200),5);



	x = 20;
	y = HEIGHT / 2;

	speed = 7;
	boundx = al_get_bitmap_width(image);
	boundy = al_get_bitmap_height(image);

}
void player::DrawPlayer()
{
    al_draw_bitmap(image, x, y, 0);

    // Draw a red marker on the edge based on lastDir
    ALLEGRO_COLOR red = al_map_rgb(255, 100, 100);
    int thickness = 3;
    switch(lastDir) {
        case 0: // Up
            al_draw_filled_rectangle(x, y, x+boundx, y+thickness, red);
            break;
        case 1: // Down
            al_draw_filled_rectangle(x, y+boundy-thickness, x+boundx, y+boundy, red);
            break;
        case 2: // Left
            al_draw_filled_rectangle(x, y, x+thickness, y+boundy, red);
            break;
        case 3: // Right
            al_draw_filled_rectangle(x+boundx-thickness, y, x+boundx, y+boundy, red);
            break;
    }
}
void player::MoveUp()
{
	y -= speed;
	if(y < 0) y = 0;
    lastDir = 0;
}
void player::MoveDown(int HEIGHT)
{
	y += speed;
	if(y > HEIGHT-boundy) y = HEIGHT-boundy;
    lastDir = 1;
}
void player::MoveLeft()
{
	x -= speed;
	if(x < 0) x = 0;
    lastDir = 2;
}
void player::MoveRight(int WIDTH)
{
	x += speed;
	if(x > WIDTH-boundx) x = WIDTH-boundx;
    lastDir = 3;
}

void player::setPosition(int newX, int newY) {
	x = newX;
	y = newY;
}