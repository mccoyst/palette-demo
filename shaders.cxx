#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

static void check_status(GLuint);

GLint init_shaders(){
	const GLchar* vsrc =
		"#version 150\n"
		"in vec2 position;"
		"in vec2 texcoord;"
		"out vec2 ftex;"
		"void main() {"
		"	gl_Position = vec4(position, 0.0, 1.0);"
		"	ftex = texcoord;"
		"}";
	const GLchar* fsrc =
		"#version 150\n"
		"in vec2 ftex;"
		"out vec4 outColor;"
		"uniform sampler2D sampo;"
		"void main() {"
		"	vec4 c = texture(sampo, ftex);"
		"	if(c.r == 1.0) outColor = vec4(0.5, 0.25, 0.25, 1.0);"
		"	else if(c.g == 1.0) outColor = vec4(0.25, 0.5, 0.25, 1.0);"
		"	else if(c.b == 1.0) outColor = vec4(0.25, 0.25, 0.5, 1.0);"
		"	else outColor = c;"
		"}";

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
	glBindFragDataLocation(p, 0, "outColor");
	glLinkProgram(p);
	GLint ok = 0;
	glGetProgramiv(p, GL_LINK_STATUS, &ok);
	if(ok){
		glUseProgram(p);
		return p;
	}

	GLint n = 0;
	glGetProgramiv(p, GL_INFO_LOG_LENGTH, &n);
	char *msg = new char[n+1];
	glGetProgramInfoLog(p, n, nullptr, msg);
	std::cerr << "program: " << msg << '\n';
	exit(1);
	return 0;
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
	std::cerr << "shader: " << msg << '\n';
	exit(1);
}

