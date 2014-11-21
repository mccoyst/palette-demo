#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

GLint init_shaders();
void load_texture(const char*);

int main(int argc, char *argv[]){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cerr << "SDL_Init: " << SDL_GetError() << '\n';
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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

	std::cerr << glGetString(GL_VERSION) << '\n';
	std::cerr << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	auto p = init_shaders();

	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat verts[] = {
		0.0, 0.5,
		0.5, -0.5,
		-0.5, -0.5,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	GLint pos = glGetAttribLocation(p, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	SDL_GL_SwapWindow(win);

	SDL_Delay(1000);
	
	return 0;
}
