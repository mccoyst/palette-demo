#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

static void check_status(GLuint);

GLint init_shaders(){
	const GLchar* vsrc =
		"#version 410 core\n"
		"in vec2 position;"
		"in vec2 texcoord;"
		"out vec2 Texcoord;"
		"void main() {"
		"	Texcoord = texcoord;"
		"	gl_Position = vec4(position, 0.0, 1.0);"
		"}";
	const GLchar* fsrc =
		"#version 150 core\n"
		"in vec2 Texcoord;"
		"out vec4 outColor;"
		"uniform sampler2D sampo;"
		"void main() {"
		"	outColor = vec4(1.0,1.0,1.0,1.0);//texture(sampo, Texcoord);\n"
		"}";
	std::cerr << glGetString(GL_VERSION) << '\n';
	std::cerr << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsrc, NULL);
	glCompileShader(vs);
	check_status(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsrc, NULL);
	glCompileShader(fs);
	check_status(fs);

	GLuint p = glCreateProgram();
	glAttachShader(p, vs);
	glAttachShader(p, fs);
	glLinkProgram(p);
	glUseProgram(p);

	return p;
}

static void check_status(GLuint s){
	GLint ok = 0;
	glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
	if(ok){
		return;
	}

	GLint n = 0;
	glGetShaderiv(s, GL_INFO_LOG_LENGTH, &n);
	char *msg = new char[n+1];
	glGetShaderInfoLog(s, n, nullptr, msg);
	std::cerr << "vertex shader: " << msg << '\n';
	exit(1);
}

