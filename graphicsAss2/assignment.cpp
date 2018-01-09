/*
Dan Ward 30 / 10 / 17

Main class for this assignment.
*/
#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#pragma comment(lib, "glloadD.lib")
#else
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glload.lib")
#endif
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "soil.lib")


#include "wrapper_glfw.h" //Yours
#include <iostream>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include "glm/gtc/random.hpp"
#include <stack>
#include "Quad.h" //Mine
#include "HeightMapping.cpp" //Mine
#include "Particle.h" //Mine
#include "soil.h"

GLuint program; //Shader prog
GLuint vao; //VAO
GLuint colourmode; //shader mode changer
GLuint drawmode;

//Uniforms
GLuint modelID, viewID, projectionID, lightPosID;
GLuint colourmodeID;
GLuint displacement;

GLfloat aspect_ratio; //for reshape callback

using namespace std;
using namespace glm;

//Window stuff
int width = 640, height = 480;

//Objects
int terAmount = 2500;
Quad terrain[2500];
GLfloat heightMap[51][51];
HeightMapping hm;
GLuint texture;
GLuint disp = 100;
Particle p[100]; 


//Control stuff
GLfloat xPos, yPos;
GLfloat horizontalAngle, verticalAngle, mouseSpeed, deltaTime, prevTime, movSpeed, rate, direc, ang;
GLfloat h, m, sec;
double xmpos, ympos;
GLFWwindow *w;
vec3 eye = vec3(0, 0, 2);
vec3 dir = vec3(0, 0, -1);
vec3 rightVec = vec3(0, 0, 0);

//Your reshape function
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = float(w) / float(h);
	glfwGetWindowSize(window, &width, &height);
}
/*
Key handling, kinda stick with the movement should be a way to smooth it out more?
*/
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W)
	{
		eye += dir * deltaTime * movSpeed;
	}

	if (key == GLFW_KEY_S)
	{
		eye -= dir * deltaTime * movSpeed;
	}

	if (key == GLFW_KEY_A)
	{
		eye -= rightVec * deltaTime * movSpeed;
	}

	if (key == GLFW_KEY_D)
	{
		eye += rightVec * deltaTime * movSpeed;
	}


	if (key == GLFW_KEY_SPACE) //Reset view angle, not used anymore was for use before I put a cap on how far up and down you could see.
	{
		verticalAngle = 0.0f;
		horizontalAngle = 3.0f;
	}

}

/*
Very simple output to console so you can see what the controls are.
*/
void outputControls()
{
	cout << "Use WASD to move around, becareful as you clip through the walls, so stay in the room!" << endl;
	cout << "Use the mouse to look around." << endl;
	cout << "Pressing the spacebar resets your view." << endl;
}

/*
Initilizes the control variables.
*/
void controlInit()
{
	outputControls();
	xPos = 0.0f;
	yPos = 0.0f;
	horizontalAngle = 0.0f;
	verticalAngle = 0.0f;
	mouseSpeed = 0.1f;
	deltaTime = 0.0f;
	prevTime = 0.0f;
	movSpeed = 1.0f;
	eye = vec3(4,2,4);
}

/*
Without running this, you very easily get a upside down view and gets very confusing not knowing which way is up.
*/
void constrainView()
{
	if (verticalAngle < -0.45f)
	{
		verticalAngle = -0.45f;
	}
	else if (verticalAngle > 0.35f)
	{
		verticalAngle = 0.35f;
	}
}
/*
Fills up the heighmap from the heightmap class.
*/
void fillHeightMap()
{
	for (int x = 0; x < sqrt(terAmount); x++)
	{
		for (int y = 0; y < sqrt(terAmount); y++)
		{
			heightMap[x][y] = hm.getHeight(x,y);
		}
	}
}

/*
Draws the terrain.
Because this draws 2500 times, this is inefficient, if I had to redo it, I would make a terrain class like a quad class would be.
*/
void renderTerrain()
{
	stack<mat4> model;
	model.push(mat4(1.0f));

	for each (Quad ter in terrain)
	{
		model.push(model.top());//Back
		{
			model.top() = scale(model.top(), vec3(0.5f, 2.0f, 0.5f));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
			ter.drawQuad(1);
		}
		model.pop();
	}
}

glm::vec4 getColour(float in)
{

	if (in < 0.1f)
	{
		return vec4(0, 0, 1, 1);
	}
	else if(in > 0.1f && in < 0.2f)
	{
		return vec4(0, 0.25f, 1, 1);
	}
	else if (in > 0.2f && in < 0.3f)
	{
		return vec4(0.75, 0.75f, 0, 1);
	}
	else if (in > 0.3f && in < 0.4f)
	{
		return vec4(0.50, 0.75f, 0.15f, 1);
	}
	else if (in > 0.4f && in < 0.5f)
	{
		return vec4(0.50, 0.75f, 0.25f, 1);
	}
	else if (in > 0.5f && in < 0.6f)
	{
		return vec4(0.25, 0.25f, 0.0f, 1);
	}
	else if (in > 0.6f && in < 0.7f)
	{
		return vec4(0.64f, 0.64f, 0.64f, 1);
	}
	else if (in > 0.7f && in < 0.8f)
	{
		return vec4(0.75f, 0.75f, 0.75f, 1);
	}
	else if (in > 0.8f && in < 0.9f)
	{
		return vec4(0.85f, 0.85f, 0.85f, 1);
	}
	else if (in > 0.9f && in < 1.0f)
	{
		return vec4(1.0f, 1.0f, 1.0f, 1);
	}

	glm::vec4 col = vec4(0, 0, 0, 1);
	return col; //black for error
}


/*
Just grabs a height from the height map previously generated.
*/
void buildTerrain()
{
	int count = 0;
	for (int x = 0; x < sqrt(terAmount); x++)
	{
		for (int y = 0; y < sqrt(terAmount); y++)
		{
			vec3 pos[4];
			pos[0] = vec3(x, heightMap[x][y], y);
			pos[1] = vec3(x, heightMap[x][y + 1], y + 1);
			pos[2] = vec3(x + 1, heightMap[x + 1][y + 1], y + 1);
			pos[3] = vec3(x + 1, heightMap[x + 1][y], y);
			terrain[count].makeQuad(pos);

			float height = (pos[0].y + pos[1].y + pos[2].y + pos[3].y) / 4;
			GLfloat colour = (height - hm.bottom) / (hm.top - hm.bottom);

			terrain[count].changeColour(getColour(colour));
			count++;
		}
	}
}

/*
Initialises the particles.
*/
void makeParticles()
{
	for(int i = 0; i < 100; i++){
		p[i].makeParticle();
		p[i].randomPosition(vec3(1,1,1)); //At this poiint eye is not real?
	}

}

void init(GLWrapper *glw)
{
	//control stuff init
	controlInit();
	glGenVertexArrays(1, &vao); 
	glBindVertexArray(vao);

	try
	{
		program = glw->LoadShader("vertex.vert", "pixel.pixel");
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	modelID = glGetUniformLocation(program, "model");
	colourmodeID = glGetUniformLocation(program, "colourmode");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightPosID = glGetUniformLocation(program, "lightpos");

	//
	displacement = glGetUniformLocation(program, "disp");

	try
	{
		/* Activate GL_TEXTURE0 (not really necessary as this is the default) */
		glActiveTexture(GL_TEXTURE0);

		/* load an image file directly as a new OpenGL texture */
		texture = SOIL_load_OGL_texture("images/displacement.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);

		/* check for an error during the load process */
		if (texture == 0)
		{
			printf("TexID1: SOIL loading error: '%s'\n", SOIL_last_result());
		}

		/* Standard bit of code to enable a uniform sampler for our texture */
		int loc1 = glGetUniformLocation(program, "tex1");
		if (loc1 >= 0) glUniform1i(loc1, 1);
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	fillHeightMap();
	buildTerrain();

	makeParticles();

}
/*
Draws the particles.
*/
void drawParticles()
{
	stack<mat4> model;
	model.push(mat4(1.0f));

	for(int i = 0; i < 100; i++)
	{
		model.push(model.top());
		{

			if (p[i].pos.y < 1.0f) //Check its not too slow.
			{
				p[i].randomPosition(eye);
			}
			p[i].decreaseParticle(); //Move it
			model.top() = translate(model.top(), p[i].pos); //From its own stored position.
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
			p[i].drawParticle();
		}
		model.pop();
	}

}

void render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program);

	ang += direc;

	mat4 projection = perspective(30.0f, 1.333f, 0.1f, 100.0f);

	glfwGetCursorPos(w, &xmpos, &ympos); 
	glfwSetCursorPos(w, width / 2, height / 2);

	deltaTime = glfwGetTime() - prevTime;
	prevTime = glfwGetTime();

	horizontalAngle += mouseSpeed * deltaTime * float(width / 2 - xmpos);
	verticalAngle += mouseSpeed * deltaTime * float(height / 2 - ympos);
	constrainView();

	 dir = vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle));

	rightVec = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	
	vec3 up = vec3(0, 1, 0);

	eye.y = (hm.getHeight(eye.x * 2, eye.z * 2) * 2) + 0.25f;

	mat4 view = lookAt(
		eye,
		eye + dir,
		up
	);

	vec4 lightpos = view * vec4(0, 0, 0, 1);

	glUniform1ui(colourmodeID, colourmode);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
	glUniform4fv(lightPosID, 1, value_ptr(lightpos));
	disp += 1;
	glUniform1ui(displacement, disp); //But doesnt get passed down as anything other than 1 

	renderTerrain();
	
	glPointSize(5); //makes the points bigger, thought about randomly changing this value, but wasnt sure how resource intensive that would be.
	drawParticles();

	glDisableVertexAttribArray(0);
	glUseProgram(0);
	
}

int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(640, 480, "Daniel Ward Assignment 2");
	w = glw->getWindow();
	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	hm = HeightMapping();
	
	// callbacks
	glw->setRenderer(render);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	/* Output the OpenGL vendor and version */
	glw->displayVersion();

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}