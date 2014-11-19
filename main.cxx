#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

int main(int argc, char *argv[]){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cerr << "SDL_Init: " << SDL_GetError() << '\n';
		return 1;
	}

	auto win = SDL_CreateWindow("palette?",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI);
	if(!win){
		std::cerr << "SDL_CreateWindow: " << SDL_GetError() << '\n';
		return 1;
	}

	SDL_GL_CreateContext(win);
	auto err = glewInit();
	if(err != GLEW_OK){
		std::cerr << "glewinit: " << glewGetErrorString(err) << '\n';
		return 1;
	}

	auto rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if(!rend){
		std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << '\n';
		return 1;
	}

	auto surf = IMG_Load("meow.png");
	if(!surf){
		std::cerr << "IMG_Load: " << IMG_GetError() << '\n';
		return 1;
	}

	auto tex = SDL_CreateTextureFromSurface(rend, surf);
	if(!tex){
		std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError() << '\n';
		return 1;
	}
	SDL_FreeSurface(surf);

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_Rect dr = {
		0, 0,
		32, 32,
	};
	if(SDL_RenderCopy(rend, tex, nullptr, &dr) < 0){
		std::cerr << "SDL_RenderCopy: " << SDL_GetError() << '\n';
		return 1;
	}
	SDL_GL_SwapWindow(win);

	SDL_Delay(1000);
	
	return 0;
}
