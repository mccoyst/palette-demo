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
	glewExperimental = GL_TRUE; 
	auto err = glewInit();
	if(err != GLEW_OK){
		std::cerr << "glewinit: " << glewGetErrorString(err) << '\n';
		return 1;
	}

	std::cerr << glGetString(GL_VERSION) << '\n';
	std::cerr << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
	for(;;){
		auto err = glGetError();
		if(err == GL_NO_ERROR){
			break;
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat verts[] = {
		-0.5f,  0.5f, 0.0f, 0.0f,
		0.5f,  0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f,
	};

	GLuint elems[] = { 0, 1, 2, 2, 3, 0 };

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);

	auto p = init_shaders();

	GLint pos = glGetAttribLocation(p, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat[4]), 0);

	GLint texcoord = glGetAttribLocation(p, "texcoord");
	glEnableVertexAttribArray(texcoord);
	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat[4]), (void*)sizeof(GLfloat[2]));

	load_texture("meow.png");
	glUniform1i(glGetUniformLocation(p, "sampo"), 0);

	glClearColor(0.5,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(win);

	SDL_Delay(1000);
	
	return 0;
}
