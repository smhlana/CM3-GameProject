#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>				//Our primitive header file
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

enum KEYS{ UP, DOWN, LEFT, RIGHT };

int main(void)
{
	int width = 640;
	int height = 480;

	bool done = false;
	bool redraw = true;

	int x = width / 2;
	int y = height / 2;

	int FPS = 60;

	bool keys[4] = { false, false, false, false };

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_SAMPLE *sample = NULL;

	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(width, height);			//create our display object

	if (!display)										//test display object
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();

	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(8);

	sample = al_load_sample("asteroids_thrust.wav");

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

	al_start_timer(timer);
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;

			int sx = x;
			int sy = y;

			if (keys[UP])
			{
				y -= 5;
				if (y <= 0) y = height;
			}
			if (keys[DOWN])
			{
				y += 5;
				if (y >= height) y = 0;
			}
			if (keys[LEFT])
			{
				x -= 5;
				if (x <= 0) x = width;
			}
			if (keys[RIGHT])
			{
				x += 5;
				if (x >= width) x = 0;
			}

			if (sx != x || sy != y)
				al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			al_draw_filled_rectangle(x - 10, y - 10, x + 10, y + 10, al_map_rgb(255, 0, 255));
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_sample(sample);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						//destroy our display object

	return 0;
}
