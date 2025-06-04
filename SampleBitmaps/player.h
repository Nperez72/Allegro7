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
	// Helper method setting new position after collision
	void setPosition(int newX, int newY);
	// Helper methods to mark correct part of the image
	void setLastDir(int dir) { lastDir = dir; }
	int getLastDir() const { return lastDir; }
private:
	int x;
	int y;
	int speed;
	int boundx;
	int boundy;
	ALLEGRO_BITMAP *image;
	int lastDir; // 0=up, 1=down, 2=left, 3=right
};
#endif

