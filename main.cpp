#include"ShaderLoader.h"
#include "SDL.h"
#include <stdio.h>

GLuint programObject;
GLuint vPos;
GLuint vao; //vertex array object
GLuint buffer;

float vertices[] = { -1.0, -1.0, 0.0, 1.0,
					1.0, -1.0, 0.0, 1.0,
					0.0, 1.0, 0.0, 1.0};


bool Running = true;
SDL_Surface* Surf_Display = NULL;

void setupVAO(){

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,	sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(vPos);
	glVertexAttribPointer(
		vPos,				// attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		0,					// normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
}


void update() {

}

void draw() {
	glClear( GL_COLOR_BUFFER_BIT );

	// Use our shader
	glUseProgram(programObject);


	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);



	SDL_GL_SwapBuffers();
}

void cleanup() {
    SDL_FreeSurface(Surf_Display);
    SDL_Quit();
}

void processEvent(SDL_Event* Event) {
    if(Event->type == SDL_QUIT) {
        Running = false;
    }
}

bool init() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    if((Surf_Display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
        return false;
    }

		GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

    glClearColor(0, 0, 0.3, 0);

    glViewport(0, 0, 640, 480);

    glEnable(GL_TEXTURE_2D);

	// Create and compile our GLSL program from the shaders
	programObject = ShaderLoader::createShader( "vertexTest.txt", "fragmentTest.txt" );
	vPos = glGetAttribLocation(programObject, "position");

	setupVAO();

    return true;
}

int execute() {
    if(init() == false) {
        return -1;
    }

    SDL_Event Event;

    while(Running) {
        while(SDL_PollEvent(&Event)) {
            processEvent(&Event);
        }

        update();
        draw();
    }

    cleanup();

    return 0;
}

int main(int argc, char **argv){

	execute();
	return 0;
}
