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

#include "wrapper_glfw.h" //Yours
#include <iostream>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include "sphere.h" //Yours
#include "cube.h" //Yours
#include "Quad.h" //Mine
#include "Cylinder.h" //Mine

GLuint program; //Shader prog
GLuint vao; //VAO
GLuint colourmode; //shader mode changer
GLuint drawmode;

//Uniforms
GLuint modelID, viewID, projectionID, lightPosID;
GLuint colourmodeID;

GLfloat aspect_ratio; //for reshape callback

using namespace std;
using namespace glm;

//Window stuff
int width = 640, height = 480;

//Objects
Quad back, rfloor, rleft, rright, rtop, rfront;
Sphere testSphere;
Cylinder clockContainer, clockFace, hand;

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

	if (key == GLFW_KEY_C) //Change colour of the 'roof' to white.
	{
		rtop.changeColour(vec4(1, 1, 1, 1));
	}

	if (key == GLFW_KEY_SPACE) //Reset view angle, not used anymore was for use before I put a cap on how far up and down you could see.
	{
		verticalAngle = 0.0f;
		horizontalAngle = 3.0f;
	}

	if (key == GLFW_KEY_R) //NOTE: using this increases hours an mintutes at the same rate as seconds so thie will distort the clock fast.
	{
		h++;
		m++;
		sec++;
	}

	if (key == GLFW_KEY_T) //NOTE: using this increases hours an mintutes at the same rate as seconds so thie will distort the clock fast.
	{
		direc++;
	}

	if (key == GLFW_KEY_G) //NOTE: using this increases hours an mintutes at the same rate as seconds so thie will distort the clock fast.
	{
		direc--;
	}

}

/*
Very simple output to console so you can see what the controls are.
*/
void outputControls()
{
	cout << "Use WASD to move around, becareful as you clip through the walls, so stay in the room!" << endl;
	cout << "Use the mouse to look around." << endl;
	cout << "The 'C' key changes the colour of the roof to white." << endl;
	cout << "The 'R' adds an hour, a miunte and a second to their respective counts." << endl;
	cout << "The 'T' increases the rotation of the ball and the fan." << endl;
	cout << "The 'G' decreses the rotation of the ball and the fan, eventually reversing the spin." << endl;
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
	movSpeed = 5.0f;
	h = 9.0f;
	m = 32.0f;
	sec = 0.0f;
	rate = 1;
	direc = 1;
	ang = 1;
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


void buildRoom()
{

	stack<mat4> model;
	model.push(mat4(1.0f));

	model.push(model.top());//Back
	{
		model.top() = translate(model.top(), vec3(-4.0f, -1.0f, -4.0f));
		model.top() = scale(model.top(), vec3(8.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		back.drawQuad(drawmode);
	}
	model.pop();

	model.push(model.top());//front
	{
		model.top() = translate(model.top(), vec3(-4.0f, -1.0f, 4.0f));
		model.top() = scale(model.top(), vec3(8.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		rfront.drawQuad(drawmode);
	}
	model.pop();

	model.push(model.top());//floor
	{
		model.top() = translate(model.top(), vec3(-4.0f, -1.0f, -4.0f));
		model.top() = scale(model.top(), vec3(8.0f, 1.0f, 8.0f));
		model.top() = rotate(model.top(), 90.0f, vec3(1, 0, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		rfloor.drawQuad(drawmode);

	}
	model.pop();

	model.push(model.top());//top
	{
		model.top() = translate(model.top(), vec3(-4.0f, 1.0f, -4.0f));
		model.top() = scale(model.top(), vec3(8.0f, 1.0f, 8.0f));
		model.top() = rotate(model.top(), 90.0f, vec3(1, 0, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		rtop.drawQuad(drawmode);

	}
	model.pop();

	model.push(model.top());//left
	{
		model.top() = translate(model.top(), vec3(-4.0f, -1.0f, 4.0f));
		model.top() = scale(model.top(), vec3(8.0f, 2.0f, 8.0f));
		model.top() = rotate(model.top(), 90.0f, vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		rleft.drawQuad(drawmode);

	}
	model.pop();

	model.push(model.top());//right
	{
		model.top() = translate(model.top(), vec3(4.0f, -1.0f, 4.0f));
		model.top() = scale(model.top(), vec3(8.0f, 2.0f, 8.0f));
		model.top() = rotate(model.top(), 90.0f, vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		rleft.drawQuad(drawmode);

	}
	model.pop();
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

	back.makeQuad(); rfloor.makeQuad();
	rleft.makeQuad(); rright.makeQuad();
	rtop.makeQuad(); rfront.makeQuad();
	testSphere.makeSphere(200,200);

	rleft.changeColour(vec4(0.6f, 1, 0.6f, 1));
	rright.changeColour(vec4(0.6f, 1, 0.6f, 1));
	back.changeColour(vec4(0.6f, 1, 0.6f, 1));
	rfront.changeColour(vec4(0.6f, 1, 0.6f, 1));

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
	buildRoom();
	stack<mat4> model;
	model.push(mat4(1.0f));
	model.push(model.top()); 
	{
		model.top() = rotate(model.top(), ang, vec3(0.0f, 0.5f, 0.0f)); //This order is a way to make things move in circles, wish I did this before the clocks :P
		model.top() = translate(model.top(), vec3(0.9f, 0.0f, 0.0f));
		model.top() = scale(model.top(), vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		testSphere.drawSphere(drawmode);
	}
	model.pop();

	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(640, 480, "Template");;
	w = glw->getWindow();
	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

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