#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

GLint init_shaders();
void load_texture(const char*);

void print_glerr(GLenum err, int line){
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

void check_gl(int line){
	auto err = glGetError();
	if(err == GL_NO_ERROR){
		return;
	}
	print_glerr(err, line);
	exit(1);
}

#define check() check_gl(__LINE__)

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
	glewExperimental = GL_TRUE; 
	auto err = glewInit();
	if(err != GLEW_OK){
		std::cerr << "glewinit: " << glewGetErrorString(err) << '\n';
		return 1;
	}

	std::cerr << glGetString(GL_VERSION) << '\n';
	std::cerr << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
	std::cerr << "residual errors:\n";
	for(;;){
		auto err = glGetError();
		if(err == GL_NO_ERROR){
			break;
		}
		print_glerr(err, __LINE__);
	}
	std::cerr << "...\n";

	GLfloat verts[] = {
		-0.5f,  0.5f,
		0.5f,  0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f,
	};

	GLuint elems[] = { 0, 1, 2, 2, 3, 0 };

	GLuint vao;
	glGenVertexArrays(1, &vao);
	check();
	glBindVertexArray(vao);
	check();

	GLuint vbo;
	glGenBuffers(1, &vbo);
	check();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	check();
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	check();

	GLuint ebo;
	glGenBuffers(1, &ebo);
	check();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	check();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);

	auto p = init_shaders();
	check();

	GLint pos = glGetAttribLocation(p, "position");
	check();
	glEnableVertexAttribArray(pos);
	check();
	glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, 0);
	check();

	glClearColor(0.5,0,0,1);
	check();
	glClear(GL_COLOR_BUFFER_BIT);
	check();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	check();
	SDL_GL_SwapWindow(win);

	SDL_Delay(1000);
	
	return 0;
}
