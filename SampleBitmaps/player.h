#ifndef PLAYERH
#define PLAYERH
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

class player
{
public:
	player(int HEIGHT);
	~player();
	void DrawPlayer();
	void MoveUp();
	void MoveDown(int HEIGHT);
	void MoveLeft();
	void MoveRight(int WIDTH);
	int getBoundX(){return boundx;}
	int getBoundY(){return boundy;}
	int getX(){return x;}
	int getY(){return y;}
private:
	int x;
	int y;
	int speed;
	int boundx;
	int boundy;
	ALLEGRO_BITMAP *image;
};
#endif

