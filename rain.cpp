#include <stdio.h>
#include <SDL2/SDL.h>

SDL_Rect drop;
SDL_Renderer *renderer = NULL;
const int width = 600;
const int height = 1200;
const int dropCount = 400;


class raindrop
{
public:
raindrop();
void update();
	
private:
int x, y, size, vel;

};

raindrop::raindrop( ){
	x = rand() % width;
	y = rand() % height;
	size = rand() % (width / 70);
	vel = rand() % 15;

}

void raindrop::update(){
	vel += 1;
	vel = (vel >= 5 ? 5 : vel);
	vel *= size;
	y += vel;
	if (y > height){
		y = 0;
		vel = 0;
	}
	
	drop = {x,y,size,size * 4};
	
	SDL_SetRenderDrawColor( renderer, 0x90, 0x90, 0xa0 + 10 * size, 0xFF);
	SDL_RenderFillRect(renderer, &drop);
}

int main(int argc, char *argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("init failed!");
	}
	SDL_Window *window = SDL_CreateWindow("a wibdow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	if(window == NULL){
		printf("window cretin failed");
	}
	renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		printf("renderer creation failed");
	}
	raindrop * rain[dropCount];
	for(int r = 0; r < dropCount; ++r){
		rain[r] = new raindrop();
	}
//	raindrop *a = new raindrop(30, 60, 30, 12);
	while(true){
		SDL_SetRenderDrawColor( renderer, 0x20, 0x20, 0x20, 0xFF);
		SDL_RenderClear(renderer);
		
			for(int r = 0; r < dropCount; ++r){
				rain[r]->update();
			}
		SDL_RenderPresent(renderer);
		SDL_Delay(20);
	}
	printf("good 2 go!");
	
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;
}