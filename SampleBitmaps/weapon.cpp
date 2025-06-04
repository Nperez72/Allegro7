#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include "weapon.h"


weapon::~weapon()
{
	al_destroy_bitmap(image);
}
weapon::weapon()
{
	speed = 10;
	live = false;
	image = al_create_bitmap(64,64);   
	if(!image) {
		exit(1);
	}

	al_set_target_bitmap(image);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(0,25,64,39,al_map_rgb(0, 255, 255));
	al_draw_filled_rectangle(25,0,39,64,al_map_rgb(0, 255, 255));		
	al_draw_circle(32,32,8,al_map_rgb(100, 100, 100),5);

	al_draw_line(0,32,64,32,al_map_rgb(100, 100, 255),2);
	al_draw_line(32,0,32,64,al_map_rgb(100, 100, 255),2);
	al_draw_circle(32,32,16,al_map_rgb(200, 200, 200),5);
	x = 0;
	y = 0;

	speed = 7;
	boundx = al_get_bitmap_width(image)/2;
	boundy = al_get_bitmap_height(image)/2;
	live=false;
	angle=0;

	// Initialize velocity
	vx = 0;
	vy = 0;
}
void weapon::Drawweapon()
{

	if(live){
		al_draw_scaled_rotated_bitmap(image,32,32,x,y,.5,.5,angle,0);
		angle+=.1;
	}
}
void weapon::Fireweapon(player &Player)
{
	if(!live)
	{
		// Center the projectile on the player
		x = Player.getX() + Player.getBoundX()/2;
		y = Player.getY() + Player.getBoundY()/2;

		// Set velocity based on player's last direction
		switch(Player.getLastDir()) {
			case 0: // Up
				vx = 0;
				vy = -speed;
				break;
			case 1: // Down
				vx = 0;
				vy = speed;
				break;
			case 2: // Left
				vx = -speed;
				vy = 0;
				break;
			case 3: // Right
				vx = speed;
				vy = 0;
				break;
			default:
				vx = speed;
				vy = 0;
				break;
		}
		live = true;
	}
}
void weapon::Updateweapon(int WIDTH)
{
	if(live)
	{
		x += vx;
		y += vy;
		// Out of bounds check for fired projectile
		if(x < 0 || x > WIDTH || y < 0)
			live = false;
	}
}
void weapon::Collideweapon(BadGuy BadGuys[], int cSize)
{
	if(live)
	{
		for(int j =0; j < cSize; j++)
		{
			if(BadGuys[j].getLive())
			{
				if(x > (BadGuys[j].getX() - BadGuys[j].getBoundX()) &&
					x < (BadGuys[j].getX() + BadGuys[j].getBoundX()) &&
					y > (BadGuys[j].getY() - BadGuys[j].getBoundY()) &&
					y < (BadGuys[j].getY() + BadGuys[j].getBoundY()))
				{
					live = false;
					BadGuys[j].setLive(false);
				}
			}
		}
	}

}

