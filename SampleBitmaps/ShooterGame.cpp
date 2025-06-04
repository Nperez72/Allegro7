#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include "player.h"
#include "BadGuy.h"
#include "weapon.h"

enum MoveDir {
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};

// Take care of the collision between the player and bad guy
bool check_player_collide(player& p, BadGuy& b);
// Take care of the collision between bad guys
bool check_badguy_collide(BadGuy& p, BadGuy& b);
// Game loop should be kept clean so all repetitive move/collide logic should be handled in a function
// Move the player in the specified direction, checking for collisons with bad guys
void MovePlayer(player& myPlayer, BadGuy* BadGuys, int numBadGuys, MoveDir dir, int WIDTH, int HEIGHT);

int main(void)
{
	const int WIDTH = 800;
	const int HEIGHT = 400;
	const int NUM_weapons = 5;
	const int NUM_BadGuyS = 5;
	enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE};
	bool keys[5] = {false, false, false, false, false};

	//primitive variable
	bool done = false;
	bool redraw = true;
	const int FPS = 60;

	//Allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	//Initialization Functions
	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if(!display)										//test display object
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_image_addon();

	//object variables
	player myPlayer(HEIGHT);
	weapon weapons[NUM_weapons];
	BadGuy BadGuys[NUM_BadGuyS];

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	srand(time(NULL));

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_set_target_bitmap(al_get_backbuffer(display));
	al_start_timer(timer);

	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			// Handle player movement and collisions within function calls
			if(keys[UP])
				MovePlayer(myPlayer, BadGuys, NUM_BadGuyS, MOVE_UP, WIDTH, HEIGHT);
			if(keys[DOWN])
				MovePlayer(myPlayer, BadGuys, NUM_BadGuyS, MOVE_DOWN, WIDTH, HEIGHT);
			if(keys[LEFT])
				MovePlayer(myPlayer, BadGuys, NUM_BadGuyS, MOVE_LEFT, WIDTH, HEIGHT);
			if(keys[RIGHT])
				MovePlayer(myPlayer, BadGuys, NUM_BadGuyS, MOVE_RIGHT, WIDTH, HEIGHT);

			for(int i=0;i<NUM_weapons;i++)
				weapons[i].Updateweapon(WIDTH);

			// Updated respawn for bad guys
			for(int i= 0;i<NUM_BadGuyS;i++)
			{
				if(!BadGuys[i].getLive())
				{
					// Only spawn if not colliding with player or any other live BadGuy
					bool overlap = false;

					// Check overlap with player
					if(check_player_collide(myPlayer, BadGuys[i])) {
						overlap = true;
					}

					// Check overlap with other live BadGuys
					for(int j = 0; j < NUM_BadGuyS; j++) {
						if(i != j && BadGuys[j].getLive() && check_badguy_collide(BadGuys[i], BadGuys[j])) {
							overlap = true;
							break;
						}
					}

					// Only attempt to spawn if no overlap
					if(!overlap) {
						BadGuys[i].StartBadGuy(WIDTH, HEIGHT);
					}
				}
			}

			for(int i=0;i<NUM_weapons;i++)
				weapons[i].Collideweapon(BadGuys, NUM_BadGuyS);
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				for(int i=0;i<NUM_weapons;i++)
					weapons[i].Fireweapon(myPlayer);
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}

		if(redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false; 

			myPlayer.DrawPlayer();
			for(int i=0;i<NUM_weapons;i++)
				weapons[i].Drawweapon();
			for(int i=0;i<NUM_BadGuyS;i++)
				BadGuys[i].DrawBadGuy();

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						//destroy our display object
	system("pause");
	return 0;
}

bool check_player_collide(player& p, BadGuy& b)
{
	// Check if the player and bad guy rectangles overlap
	// Returns true if overlap happens, false if not
	return !(p.getX() + p.getBoundX() < b.getX() ||
		p.getX() > b.getX() + b.getBoundX() ||
		p.getY() + p.getBoundY() < b.getY() ||
		p.getY() > b.getY() + b.getBoundY());
}

void MovePlayer(player& myPlayer, BadGuy* BadGuys, int numBadGuys, MoveDir dir, int WIDTH, int HEIGHT) {

	// Store the player's old position to reset if a collision occurs
	int oldX = myPlayer.getX();
	int oldY = myPlayer.getY();

	// Use enum to determine movement direction
	switch (dir) {
		case MOVE_UP:   
			myPlayer.MoveUp(); 
			break;
		case MOVE_DOWN:  
			myPlayer.MoveDown(HEIGHT); 
			break;
		case MOVE_LEFT:  
			myPlayer.MoveLeft(); 
			break;
		case MOVE_RIGHT: 
			myPlayer.MoveRight(WIDTH); 
			break;
	}

	// Check for collisions with bad guys after moving
	for (int i = 0; i < numBadGuys; ++i) {
		if (BadGuys[i].getLive() && check_player_collide(myPlayer, BadGuys[i])) {
			myPlayer.setPosition(oldX, oldY); // Reset position if collision occurs
			break;
		}
	}

}

bool check_badguy_collide(BadGuy& b1, BadGuy& b2)
{
	// Check if the bad guy rectangles overlap
	// Returns true if overlap happens, false if not
	return !(b1.getX() + b1.getBoundX() < b2.getX() ||
		b1.getX() > b2.getX() + b2.getBoundX() ||
		b1.getY() + b1.getBoundY() < b2.getY() ||
		b1.getY() > b2.getY() + b2.getBoundY());
}







