#include <SDL.h>
#include <stdio.h>
const double PI = 3.14159265358979;

const int SCREEN_WIDTH = 650;
const int SCREEN_HEIGHT = 650;
Uint32* pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
int drawMethod = 3;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Texture* texture;

class pendulum {
public:
	pendulum();
	pendulum(double, double, double, double, double, double, int);
	void draw();
int colour;
	int x1=0, y1=0, x2 = 200, y2 = 200;
	double m1, m2, l1, l2, a1, a2, g;

	double a1_p = 0, a2_p = 0, a1_dp = 0, a2_dp = 0;

};
pendulum::pendulum() {
	m1 = 40; m2 = 40;
	l1 = l2 = 150;
	a1 = a2 = PI;
	g = -0.0005;
	colour = 0xFF;
}

pendulum::pendulum(double m1_, double m2_, double l1_, double l2_, double a1_, double a2_, int colour_){
	m1 = m1_;
	m2 = m2_;
	l1 = l1_;
	l2 = l2_;
	a1 = a1_;
	a2 = a2_;
	colour = colour_;
	g = -0.0005;
}

void pendulum::draw() {
	SDL_SetRenderDrawColor(renderer, colour, colour, colour, 0xFF);
	//SDL_RenderDrawPoint(renderer, 30, 30);
	
	double term1 = -g * (2 * m1 + m2) * sin(a1);
	double term2 = -m2 * g * sin(a1 - 2 * a2);
	double term3 = -2 * sin(a1 - a2) * m2 * (a2_p*a2_p*l2+a1_p*a1_p*l1*cos(a1-a2));
	double term4 = l1 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
	a1_dp = (term1+term2+term3)/term4;

	
	term1 = 2 * sin(a1 - a2);
	term2 = a1_p * a1_p * l1 * (m1 + m2);
	term3 = g * (m1 + m2) * cos(a1) + a2_p * a2_p * l2 * m2 * cos(a1 - a2);
	term4 = l2 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
	a2_dp = (term1 * (term2 + term3)) / term4;
	
	a1_p += a1_dp;
	a2_p += a2_dp;
	//a1_p *= 0.99999;
	///a2_p *= 0.9999;

	a1 += a1_p;
	a2 += a2_p;

	x1 = l1 * sin(a1) + SCREEN_WIDTH / 2;
	y1 = -l1 * cos(a1) + SCREEN_HEIGHT / 2;
	x2 = x1 + l2 * sin(a2);
	y2 = y1 -l1 * cos(a2);
	SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, x1, y1);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	
	if (0 <= x1 && x1 <= SCREEN_WIDTH && 0 <= y2 && y2 <= SCREEN_HEIGHT) {
		switch (drawMethod) {
		default:
		case 1:
			if (pixels[y2 * SCREEN_WIDTH + x2] == 0x202020FF) {
				pixels[y2 * SCREEN_WIDTH + x2] = 0xFF000005;
			}
			else {
				pixels[y2 * SCREEN_WIDTH + x2] += 0x000F0005;
			}
			break;
		case 2:
			pixels[y2 * SCREEN_WIDTH + x2] += 0xFF027050;
			break;
		case 3:
			pixels[y2 * SCREEN_WIDTH + x2] += 0x000000FF;
			break;
		case 4:
			pixels[y2 * SCREEN_WIDTH + x2] = 0xFF0270FF;
			break;
		}
	}
	
}
const int penCount = 20;
pendulum pens[penCount];
int main(int argc, char** argv) {
	bool quit = false;
	SDL_Event e;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL init failure\n %s", SDL_GetError());
	} else {
		window = SDL_CreateWindow("Pendulum", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("window create error\n %s", SDL_GetError());
		}
		else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				printf("renderer init failed %s", SDL_GetError());
			}
			else {
				texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
				
			}
		}
	}
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT;++i) {
		pixels[i] = 0x202020FF;
	}
	
	for (int i = 0; i < penCount;++i) {
		//pens[i] = pendulum(500, 100, 150, 150, 0.001 * PI*i, PI, (0x0F*i)%255);
		pens[i] = pendulum(100, 100, 150, 150, 0.25 * PI, i*0.01*PI, 0xBB);
	}
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_q:
					quit = true;
					break;
				case SDLK_UP:
					drawMethod++;
					printf("Method: %i", drawMethod);
					break;
				case SDLK_DOWN:
					drawMethod--;
					printf("Method: %i", drawMethod);
					break;
				}
			}
		}
		//clear screen
		SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0xFF);
		//SDL_RenderClear(renderer);
		SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		for (int i = 0; i < penCount;++i) {
			pens[i].draw();
		}
		
		//
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_Quit();
	return 0;
}

