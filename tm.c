#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#define H 20
#define W 40
#define M 3
#define R 512
#define S 14
#define D 10000000
#define SCREEN_W 2400
#define SCREEN_H 1600
#define PIXEL_WIDTH 25
#define PIXEL_HEIGHT 50
int top_screen = 200, left_screen = 100;
int c[W][H] = {};
int m[M][R] = {};
typedef struct { int x; int y;} pos;
pos p[M];
int machine_choice(int);
int s[] = {32,9608};
void print_c();
void print_code(int,int,int);
void init_c();
void init_p();
void init_m();
void update_world();
void move(int,int);
void init_graphics();
void plot_pixel(int,int);
int key_pressed();
int getch();
int sit_code(int);
int mod(int,int);
int int_power(int,int);
void init_p();
void toggle(int,int);
void move_machines();
void move_machine(int);
ALLEGRO_COLOR random_color()
{
    return al_map_rgb(rand()%256, rand()%256, rand()%256);
}

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT event;
ALLEGRO_COLOR one_color, white, red, black, blue, green, yellow, gray, current_color, orange;
ALLEGRO_EVENT_QUEUE *q;

int main() {
	one_color = white;
	init_graphics();
	init_c();
	init_m();
	init_p();
	al_clear_to_color(black);
	while (getch() != ALLEGRO_KEY_Q) {
		al_clear_to_color(black);
		//one_color = random_color();
		current_color = white;
		print_c();
		//current_color = red;
		move_machines();
		al_flip_display();
		for (int d = 0; d < D; d++);
		//while(!key_pressed());
	}
}

void init_c() {
	for (int i = 0; i < W; i++) for(int j = 0; j < H; j++) c[i][j] = rand()%2;
}
void init_p() {
    for (int i = 0; i < M; i++) {
		p[i].x = rand()%W;
		p[i].y = rand()%H;
	}
}
void print_c() {
	for (int i = 0; i < W; i++) for(int j = 0; j < H; j++) {
		if (c[i][j] == 1) current_color = white; else current_color = black;
		plot_pixel(i,j);
	}
}

void init_graphics()
{
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    q = al_create_event_queue();
    al_register_event_source(q, al_get_keyboard_event_source());
    display = al_create_display(SCREEN_W, SCREEN_H);
    white = al_map_rgb(255, 255, 255);
    black = al_map_rgb(0, 0, 0);
    red = al_map_rgb(255, 0, 0);
    blue = al_map_rgb(0, 0, 255);
    green = al_map_rgb(0, 255, 0);
    yellow = al_map_rgb(255, 255, 0);
    orange = al_map_rgb(255,165,0);
}
void plot_pixel(int x, int y) {
    al_draw_filled_rectangle(
                             left_screen+PIXEL_WIDTH*x,
                             top_screen + PIXEL_HEIGHT*y,
                             left_screen+PIXEL_WIDTH*(x+1),
                             top_screen +PIXEL_HEIGHT*(y+1),
                             current_color

    );
}
int key_pressed()
{
    ALLEGRO_EVENT ev;
    while (al_peek_next_event(q, &ev))
    {
        if (ev.type != ALLEGRO_EVENT_KEY_CHAR)
        {
            al_drop_next_event(q);
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

int getch()
{
    ALLEGRO_EVENT ev;
    al_get_next_event(q, &ev);
    if (ev.type != ALLEGRO_EVENT_KEY_CHAR)
    {
        return 0;
    }
    return ev.keyboard.keycode;
}

void init_m() {
	for (int n = 0 ; n < M ; n++) for (int r = 0; r < R; r++) {
		m[n][r] = rand()%S;
	}
}

int mod(int x, int m) {
    if (x > -1 && x < m) return x;
    if (x < 0) return x + m;
    if (x > m - 1) return x - m;
	return 0;
}

int int_pow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp % 2)
           result *= base;
        exp /= 2;
        base *= base;
    }
    return result;
}

int sit_code(int n) {
	int x = p[n].x;
	int y = p[n].y;
	int z = 0;
	int sum = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			sum += c[mod(x+i,W)][mod(y+j,H)]*int_pow(2,z++);
		}
	}
	return sum;
}

void toggle(int i, int j) {
	if (c[i][j] == 1) c[i][j] = 0; else c[i][j] = 1;
}

void move_machine(int n) {
	int c = m[n][sit_code(n)];
	int x = p[n].x; int y = p[n].y;
	//if (c >= 8) toggle(x,y);
	toggle(x,y);
	switch(c%8) {
		case 0: p[n].x = mod(x - 1,W);
				p[n].y = mod(y - 1,H);
				break;
		case 1: p[n].y = mod(y - 1,H);
                break;
		case 2: p[n].x = mod(x + 1,W);
                p[n].y = mod(y - 1,H);
                break;
		case 3: p[n].x = mod(x - 1,W);
                break;
		case 4: p[n].x = mod(x + 1,W);
                break;
		case 5: p[n].x = mod(x - 1,W);
                p[n].y = mod(y + 1,H);
                break;
		case 6: p[n].y = mod(y + 1,H);
                break;
		case 7: p[n].x = mod(x + 1,W);
                p[n].y = mod(y + 1,H);
                break;
	}
	current_color = red;
	plot_pixel(p[n].x,p[n].y);
}

void move_machines() {
	for (int n = 0 ; n < M ; n++) move_machine(n);
}
