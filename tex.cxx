#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

void load_texture(const char *fname){
	auto surf = IMG_Load("meow.png");
	if(!surf){
		std::cerr << "IMG_Load: " << IMG_GetError() << '\n';
		exit(1);
	}

	int fmt = GL_BGRA;
	int ncolors = surf->format->BytesPerPixel;
	if(ncolors == 4){
		if (surf->format->Rmask == 0x000000ff)
			fmt = GL_RGBA;
		else
			fmt = GL_BGRA;
	}else if (ncolors == 3){
		if (surf->format->Rmask == 0x000000ff)
			fmt = GL_RGB;
		else
			fmt = GL_BGR;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0,
		fmt, GL_UNSIGNED_BYTE, surf->pixels);

	SDL_FreeSurface(surf);
}
