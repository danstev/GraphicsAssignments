/*
Dan Ward 30/10/17

Cylinder from scratch

1. Draw a circle
2. Those verticies are the same, just Y is +/- 0.5 for top/bottom, top has position y normal, bottom negative y normals
3. Triangle strips from top and bottom verticies, normals this time are different.
*/

using namespace std;

#include "cylinder.h"
#include <iostream>

Cylinder::Cylinder()
{
	attribute_v_coord = 0;
	attribute_v_colours = 1;
	attribute_v_normal = 2;
	numverticies = 32 *3;
}

Cylinder::~Cylinder()
{
}

void Cylinder::makeCylinder()
{
	glm::vec3 vertex[128];
	glm::vec3 normals[128];
	glm::vec4 colours[128];
	
	vertex[0] = glm::vec3(0.0f, 0.5f, 0.0f); vertex[32] = glm::vec3(0.0f,-0.5f, 0.0f);
	normals[0] = glm::vec3(0.0f, 1.0f, 0.0f); normals[32] = glm::vec3(0.0f, 1.0f, 0.0f);
	colours[0] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); colours[32] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	int offset = 0;
	for (int i = 1; i < 32; i++)
	{
		GLfloat angle = 2 * 3.1415926f / ((float)30 / (float)i);

		vertex[i] = glm::vec3(0.5f * cos(angle), 0.5f, 0.5f * sin(angle));
		vertex[i + 32] = glm::vec3(0.5f * cos(angle), -0.5f, 0.5f * sin(angle));
		vertex[i + 64 + offset] = vertex[i];
		vertex[i + 64 + 1 + offset] = vertex[i + 32];
		normals[i] = glm::vec3(0.0f, 1.0f, 0.0f);
		normals[i + 32] = glm::vec3(0.0f, -1.0f, 0.0f);
		normals[i + 64 + offset] = glm::vec3(cos(angle), 0.0f, sin(angle)); //I think I did this wrong, for some reason in testing, I get golden colours (this is because its slightly offset wrong).
		normals[i + 64 + 1 + offset] = glm::vec3(cos(angle), 0.0f, sin(angle)); //Also is the sin/cos function very costly? worth it to just  = vertex[i].x etc instead? //Im convinced the problem here is that it takes the bottom circle center in as well.
		colours[i] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		colours[i + 32] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//colours[i + (32 * 2)] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); //This breaks the program?
		//colours[i + (32 * 3)] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		offset++;
	}

	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) , vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &colourObject);
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours) * 4, colours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals) * 3, normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Cylinder::changeColour(glm::vec4 col)
{
	glm::vec4 colours[128];

	for (int i = 0; i < numverticies; i++)
	{
		colours[i] = col;
	}


	/* Create the colours buffer for the cube */
	glGenBuffers(1, &colourObject);
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Cylinder::drawCylinder()
{
	/* Bind cube vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(attribute_v_coord);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index 1 */
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glEnableVertexAttribArray(attribute_v_colours);
	glVertexAttribPointer(attribute_v_colours, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube normals. Note that this is in attribute index 2 */
	glEnableVertexAttribArray(attribute_v_normal);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 32);
	glDrawArrays(GL_TRIANGLE_FAN, 33, 64);
	glDrawArrays(GL_TRIANGLE_STRIP, 65, 64);
}