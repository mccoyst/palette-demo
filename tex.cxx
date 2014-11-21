#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

static void print_glerr(GLenum err, int line){
#define x(e) case e: std::cerr << __FILE__ << ':' << line << ": " << #e << '\n'; break;
	switch(err){
	x(GL_INVALID_ENUM)
	x(GL_INVALID_VALUE)
	x(GL_INVALID_OPERATION)
	x(GL_INVALID_FRAMEBUFFER_OPERATION)
	x(GL_OUT_OF_MEMORY)
	x(GL_STACK_UNDERFLOW)
	x(GL_STACK_OVERFLOW)
	}
#undef x
}

static void check_gl(int line){
	auto err = glGetError();
	if(err == GL_NO_ERROR){
		return;
	}
	print_glerr(err, line);
	exit(1);
}

#define check() check_gl(__LINE__)


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
	check();
	glBindTexture(GL_TEXTURE_2D, tex);
	check();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	check();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	check();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	check();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	check();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0,
		fmt, GL_UNSIGNED_BYTE, surf->pixels);
	check();

	SDL_FreeSurface(surf);
}
