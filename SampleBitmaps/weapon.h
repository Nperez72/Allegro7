#ifndef WEAPONH
#define WEAPONH
#include "BadGuy.h"
#include "player.h"
class weapon
{
public:
	weapon();
	~weapon();
	void Drawweapon();
	void Fireweapon(player &Player);
	void Updateweapon(int WIDTH);
	void Collideweapon(BadGuy BadGuys[], int cSize);
private:
	int x;
	int y;
	// The velocity of the weapon projectile
	int vx;
	int vy;
	int boundx;
	int boundy;
	bool live;
	int speed;
	float angle;
	ALLEGRO_BITMAP *image;
};
#endif
